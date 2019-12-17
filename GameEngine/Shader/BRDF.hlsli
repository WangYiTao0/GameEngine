#include "ShadingMath.hlsli"

// constants
#define EPSILON 0.000000000001f

// defines
#define _DEBUG

// ================================== BRDF NOTES =========================================
//	src: https://learnopengl.com/#!PBR/Theory
//	
// Rendering Equation
// ------------------
//
//		L_o(p, w_o) = L_e(p) + Integral_Omega()[f_r(p, w_i, w_o) * L_i(p, w_i) * dot(N, w_i)]dw_i
//
// L_o		: Radiance leaving surface point P along the direction w_o (V=eye)
// L_e		: Emissive Radiance leaving surface point p | we're not gonna use emissive materials for now 
// L_i		: Irradiance (Radiance coming) coming from a light source at point p 
// f_r()	: reflectance equation representing the material property
// Integral : all incoming radiance over hemisphere omega, reflected towards the direction of w_o<->(V=eye) 
//			  by the surface material
//
// The integral is numerically solved.
// 
// BRDF						: Bi-directional Reflectance Distribution Function
// The Cook-Torrence BRDF	: f_r = k_d * f_lambert + k_s * f_cook-torrence
//							f_lambert = albedo / PI;
//
struct BRDF_Surface
{
    float3 N;
    float roughness;
    float3 diffuseColor;
    float metalness;
    float3 specularColor;
    float3 emissiveColor;
    float emissiveIntensity;
};

// Trowbridge-Reitz GGX Distribution
// Normal Distribution  
inline float NormalDistributionGGX(float3 N, float3 H, float roughness)
{ // approximates microfacets :	approximates the amount the surface's microfacets are
	//								aligned to the halfway vector influenced by the roughness
	//								of the surface
	//							:	determines the size, brightness, and shape of the specular highlight
	// NDF_GGXTR(N, H, roughness) = roughness^2 / ( PI * ( dot(N, H))^2 * (roughness^2 - 1) + 1 )^2
    const float a = roughness * roughness;
    const float a2 = a * a;
    const float nh2 = pow(max(dot(N, H), 0), 2);
    const float denom = (PI * pow((nh2 * (a2 - 1.0f) + 1.0f), 2));
    if (denom < EPSILON)
        return 1.0f;
#if 0
	return min(a2 / denom, 10);
#else
    return a2 / denom;
#endif
}

// Smith's Schlick-GGX for Direct Lighting (non-IBL)
inline float Geometry_Smiths_SchlickGGX(float3 N, float3 V, float roughness)
{ // describes self shadowing of geometry
	//
	// G_ShclickGGX(N, V, k) = ( dot(N,V) ) / ( dot(N,V)*(1-k) + k )
	//
	// k		 :	remapping of roughness based on wheter we're using geometry function 
	//				for direct lighting or IBL
	// k_direct	 = (roughness + 1)^2 / 8
	// k_IBL	 = roughness^2 / 2
	//
    const float k = pow((roughness + 1.0f), 2) / 8.0f;
    const float NV = max(0.0f, dot(N, V));
    const float denom = (NV * (1.0f - k) + k) + 0.0001f;
	//if (denom < EPSILON) return 1.0f;
    return NV / denom;
}

// Smith's Schlick-GGX for Environment Maps
inline float Geometry_Smiths_SchlickGGX_EnvironmentMap(float3 N, float3 V, float roughness)
{ // describes self shadowing of geometry
	//
	// G_ShclickGGX(N, V, k) = ( dot(N,V) ) / ( dot(N,V)*(1-k) + k )
	//
	// k		 :	remapping of roughness based on wheter we're using geometry function 
	//				for direct lighting or IBL
	// k_direct	 = (roughness + 1)^2 / 8
	// k_IBL	 = roughness^2 / 2
	//
    const float k = pow(roughness, 2) / 2.0f;
    const float NV = max(0.0f, dot(N, V));
    const float denom = (NV * (1.0f - k) + k) + 0.0001f;
	//if (denom < EPSILON) return 1.0f;
    return NV / denom;
}

#ifdef _DEBUG
float Geometry(float3 N, float3 V, float3 L, float k)
{ // essentially a multiplier [0, 1] measuring microfacet shadowing
    float geomNV = Geometry_Smiths_SchlickGGX(N, V, k);
    float geomNL = Geometry_Smiths_SchlickGGX(N, L, k);
    return geomNV * geomNL;
}
#else
inline float Geometry(float3 N, float3 V, float3 L, float k)
{	// essentially a multiplier [0, 1] measuring microfacet shadowing
	return  Geometry_Smiths_SchlickGGX(N, V, k) * Geometry_Smiths_SchlickGGX(N, L, k);
}
#endif

float GeometryEnvironmentMap(float3 N, float3 V, float3 L, float k)
{ // essentially a multiplier [0, 1] measuring microfacet shadowing
    float geomNV = Geometry_Smiths_SchlickGGX_EnvironmentMap(N, V, k);
    float geomNL = Geometry_Smiths_SchlickGGX_EnvironmentMap(N, L, k);
    return geomNV * geomNL;
}

// Fresnel-Schlick approximation describes reflection
inline float3 Fresnel(float3 N, float3 V, float3 F0)
{ // F_Schlick(N, V, F0) = F0 - (1-F0)*(1 - dot(N,V))^5
    return F0 + (float3(1, 1, 1) - F0) * pow(1.0f - max(0.0f, dot(N, V)), 5.0f);
}


float3 FresnelWithRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max((1.0f - roughness).xxx, F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

inline float3 F_LambertDiffuse(float3 kd)
{
    return kd / PI;
}

float3 BRDF(float3 Wi, BRDF_Surface s, float3 V, float3 worldPos)
{
	// vectors
    const float3 Wo = normalize(V);
    const float3 N = normalize(s.N);
    const float3 H = normalize(Wo + Wi);

	// surface
    const float3 albedo = s.diffuseColor;
    const float roughness = s.roughness;
    const float metalness = s.metalness;
    const float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metalness);

	// Fresnel_Cook-Torrence BRDF
    const float3 F = Fresnel(H, V, F0);
    const float G = Geometry(N, Wo, Wi, roughness);
    const float NDF = NormalDistributionGGX(N, H, roughness);
    const float denom = (4.0f * max(0.0f, dot(Wo, N)) * max(0.0f, dot(Wi, N))) + 0.0001f;
    const float3 specular = NDF * F * G / denom;
    const float3 Is = specular * s.specularColor;

	// Diffuse BRDF
    const float3 kS = F;
    const float3 kD = (float3(1, 1, 1) - kS) * (1.0f - metalness) * albedo;
    const float3 Id = F_LambertDiffuse(kD);
	
    return (Id + Is);
}

float3 EnvironmentBRDF(BRDF_Surface s, float3 V, float ao, float3 irradience, float3 envSpecular, float2 F0ScaleBias)
{
    const float3 F0 = lerp(0.04f.xxx, s.diffuseColor, s.metalness);
	
    const float3 Ks = FresnelWithRoughness(max(dot(s.N, V), 0.0), F0, s.roughness);
    const float Kd = (1.0f - Ks) * (1.0f - s.metalness);
	
    const float3 diffuse = irradience * s.diffuseColor;
    const float3 specular = envSpecular * (Ks * F0ScaleBias.x + F0ScaleBias.y);

    return (Kd * diffuse + specular) * ao;
}

#define SAMPLE_COUNT 1024
#define MAX_REFLECTION_LOD 7


// Instead of uniformly or randomly (Monte Carlo) generating sample vectors over the integral's hemisphere, we'll generate 
// sample vectors biased towards the general reflection orientation of the microsurface halfway vector based on the surface's roughness. 
// This gives us a sample vector somewhat oriented around the expected microsurface's halfway vector based on some input roughness 
// and the low-discrepancy sequence value Xi. Note that Epic Games uses the squared roughness for better visual results as based on 
// Disney's original PBR research.
float3 ImportanceSampleGGX(float2 Xi, float3 N, float roughness)
{
    const float a = roughness * roughness;

    const float phi = 2.0f * PI * Xi.x;
    const float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (a * a - 1.0f) * Xi.y));
    const float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

	// from sphreical coords to cartesian coords
    float3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

	// from tangent-space to world space
    const float3 up = abs(N.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    const float3 tangent = normalize(cross(up, N));
    const float3 bitangent = cross(N, tangent);

    const float3 sample = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sample);
}

float2 IntegrateBRDF(float NdotV, float roughness)
{
    float3 V;
    V.x = sqrt(1.0f - NdotV * NdotV);
    V.y = 0;
    V.z = NdotV;

    float F0Scale = 0; // Integral1
    float F0Bias = 0; // Integral2
	
    const float3 N = float3(0, 0, 1);
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        const float2 Xi = Hammersley(i, SAMPLE_COUNT);
        const float3 H = ImportanceSampleGGX(Xi, N, roughness);
        const float3 L = normalize(reflect(-V, H));

        const float NdotL = max(L.z, 0);
        const float NdotH = max(H.z, 0);
        const float VdotH = max(dot(V, H), 0);

        if (NdotL > 0.0f)
        {
            const float G = GeometryEnvironmentMap(N, V, L, roughness);
            const float G_Vis = (G * VdotH) / ((NdotH * NdotV) + 0.0001);
            const float Fc = pow(1.0 - VdotH, 5.0f);

            F0Scale += (1.0f - Fc) * G_Vis;
            F0Bias += Fc * G_Vis;
        }
    }
    return float2(F0Scale, F0Bias) / SAMPLE_COUNT;
}
