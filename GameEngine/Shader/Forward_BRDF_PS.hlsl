#include "ShaderOptions.hlsli"
#include "CommonCbuf.hlsli"

#define PI 3.14159265359f

cbuffer Pbrlight
{
    float4 p_Position[4];
    float4 p_Color[4];

};


struct PS_IN
{
    float3 worldPos : Position;
    float3 worldNormal : Normal;
    float3 worldTan : Tangent;
    float3 worldBitan : Bitangent;
    float2 texcoord : Texcoord;
};

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metallicMap : register(t2);
Texture2D roughnessMap : register(t3);
Texture2D aoMap : register(t4);

SamplerState sampleWrap : register(s0);

cbuffer PbrLight : register (b5)
{
    float4 LightPosition[4];
    float4 LightColor[4];
};

// ----------------------------------------------------------------------------
float DistributionGGX(float3 N, float3 H, float roughness)
{

    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------

float4 main(PS_IN pIn) : SV_Target
{
    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);
    pIn.worldTan = normalize(pIn.worldTan);
    pIn.worldBitan = normalize(pIn.worldBitan);
    pIn.worldNormal = MapNormal(pIn.worldTan, pIn.worldBitan, pIn.worldNormal, pIn.texcoord, normalMap, sampleWrap);

    float3 albedo = pow(albedoMap.Sample(sampleWrap, pIn.texcoord).rgb, float3(2.2, 2.2, 2.2));
    float metallic = metallicMap.Sample(sampleWrap, pIn.texcoord).r;
    float roughness = roughnessMap.Sample(sampleWrap, pIn.texcoord).r;
    float ao = aoMap.Sample(sampleWrap, pIn.texcoord).r;

    //float3 albedo = float3(0.5f, 0.0f, 0.0f);
    //float metallic = metallicMap.Sample(sampleWrap, pIn.texcoord).r;
    //float roughness = roughnessMap.Sample(sampleWrap, pIn.texcoord).r;
    //float ao = 1.0;

    float3 N = pIn.worldNormal;
    float3 V = normalize(cameraPos - pIn.worldPos);

    
    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, metallic);

    // reflectance equation
    float3 Lo = float3(0.0, 0.0, 0.0);
    for (int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        float3 L = normalize(LightPosition[i].xyz - pIn.worldPos);
        float3 H = normalize(V + L);
        float distance = length(LightPosition[i].xyz - pIn.worldPos);
        float attenuation = 1.0 / (distance * distance);
        float3 radiance = LightColor[i].xyz * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        float3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        float3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
        float3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        float3 kD = float3(1.0,1.0f,1.0f) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    float3 ambient = float3(0.03, 0.03, 0.03) * albedo * ao;
    
    float3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + float3(1.0, 1.0, 1.0));
    // gamma correct
    color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

    float4 FragColor = float4(color, 1.0);

    return FragColor;
}