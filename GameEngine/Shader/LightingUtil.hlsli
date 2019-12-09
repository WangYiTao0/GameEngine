#define MaxLights 16

// Defaults for number of lights.
#ifndef NUM_DIR_LIGHTS
    #define NUM_DIR_LIGHTS 1
#endif

#ifndef NUM_POINT_LIGHTS
    #define NUM_POINT_LIGHTS 1
#endif

#ifndef NUM_SPOT_LIGHTS
    #define NUM_SPOT_LIGHTS 0
#endif

float4 gAmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };

struct LightVectorData
{
    float3 vToL; // vector to light
    float3 dirToL; // dir to light
    float distToL; //distance to light
};


struct Light
{
    float3 position; //spot point
    float attQuad; //point spot

    float3 diffcolor; //  direct spot point 
    float attLin; //point spot

    float3 direction; //direct spot
    float attConst; //point spot

    float3 ambient; //direct spot point 
    float spotPower; //spot

    float3 specular;
    float cutOff;//spot

    float outerCutOff;//spot
    float diffuseIntensity;//direct spot point
    float2 LightPadding;

};

cbuffer LightCB : register(b1)
{
        // Indices [0, NUM_DIR_LIGHTS) are directional lights;
    // indices [NUM_DIR_LIGHTS, NUM_DIR_LIGHTS+NUM_POINT_LIGHTS) are point lights;
    // indices [NUM_DIR_LIGHTS+NUM_POINT_LIGHTS, NUM_DIR_LIGHTS+NUM_POINT_LIGHT+NUM_SPOT_LIGHTS)
    // are spot lights for a maximum of MaxLights per object.
    Light gLights[MaxLights];
}

struct CommonMaterial
{
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float3 specularColor;
    float specularIntensity;
    float3 reflect;

};

struct Material
{
    float4 diffuseAlbedo;
    float3 fresnelR0;
    float shininess;
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 worldPos)
{
    LightVectorData lv;
    lv.vToL = lightPos - worldPos;
    lv.distToL = length(lv.vToL);
    lv.dirToL = lv.vToL / lv.distToL;
    return lv;
}


float CalcAttenuation(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
    return 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
}

float3 Diffuse(
    uniform float3 diffuse,
    uniform float diffuseIntensity,
    const in float att,
    const in float3 worldDirFragToL,
    const in float3 worldNormal)
{
    return diffuse * diffuseIntensity * att * max(0.0f, dot(worldDirFragToL, worldNormal));
}

float3 Speculate(
    const in float3 specularColor,
    uniform float specularIntensity,
    const in float3 worldNormal,
    const in float3 worldFragToL,
    const in float3 worldPos,
    const in float3 cameraPos,
    const in float att,
    const in float specularPower)
{
    // calculate reflected light vector
    const float3 w = worldNormal * dot(worldFragToL, worldNormal);
    const float3 r = normalize(w * 2.0f - worldFragToL);
    // vector from camera to fragment (in view space)
    const float3 worldCamToFrag = normalize(worldPos-cameraPos);
    // calculate specular component color based on angle between
    // viewing vector and reflection vector, narrow with power function
    return att * specularColor * specularIntensity * pow(max(0.0f, dot(-r, worldCamToFrag)), specularPower);
}

// Schlick gives an approximation to Fresnel reflectance (see pg. 233 "Real-Time Rendering 3rd Ed.").
// R0 = ( (n-1)/(n+1) )^2, where n is the index of refraction.
float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

    return reflectPercent;
}

float3 FresnelWithRoughness(float cosTheta, float3 F0, float roughness)
{
     /* const float3 F0 = lerp(0.04f.xxx, s.diffuseColor, s.metalness);
	
     const float3 Ks = FresnelWithRoughness(max(dot(s.N, V), 0.0), F0, s.roughness);
  */
    return F0 + (max((1.0f - roughness).xxx, F0) - F0) * pow(1.0 - cosTheta, 5.0);
}


float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
    const float m = mat.shininess * 256.0f;
    float3 halfVec = normalize(toEye + lightVec);

    float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(mat.fresnelR0, halfVec, lightVec);

    float3 specAlbedo = fresnelFactor * roughnessFactor;

    // Our spec formula goes outside [0,1] range, but we are 
    // doing LDR rendering.  So scale it down a bit.
    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (mat.diffuseAlbedo.rgb + specAlbedo) * lightStrength;
}



//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for directional lights.
//---------------------------------------------------------------------------------------
float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{
    // The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.direction;


    // Scale light down by Lambert's cosine law.
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.diffColor * ndotl;


    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for point lights.
//---------------------------------------------------------------------------------------
float3 ComputePointLight(Light L, Material mat, float3 worldPos, float3 worldNormal, float3 toEye)
{
    // The vector from the surface to the light.
    float3 lightVec = L.position - worldPos;

    // The distance from surface to light.
    float d = length(lightVec);

    // Range test.


    // Normalize the light vector.
    lightVec /= d;

    // Scale light down by Lambert's cosine law.
    float ndotl = max(dot(lightVec, worldNormal), 0.0f);
    float3 lightStrength = L.diffColor * ndotl;

    // Attenuate light by distance.
    float att = CalcAttenuation(L.attConst, L.attLin, L.attQuad, d);
    lightStrength *= att;


    return BlinnPhong(lightStrength, lightVec, worldNormal, toEye, mat);
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for spot lights.
//---------------------------------------------------------------------------------------
float3 ComputeSpotLight(Light L, Material mat, float3 pos, float3 normal, float3 toEye)
{
    // The vector from the surface to the light.
    float3 lightVec = L.position - pos;

    // The distance from surface to light.
    float d = length(lightVec);

    // Normalize the light vector.
    lightVec /= d;

    // Scale light down by Lambert's cosine law.
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.diffColor * ndotl;

    // Attenuate light by distance.
    float att = CalcAttenuation(L.attConst, L.attLin, L.attQuad, d);
    lightStrength *= att;

    // Scale by spotlight
    float spotFactor = pow(max(dot(-lightVec, L.direction), 0.0f), L.spotPower);
    lightStrength *= spotFactor;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}


float4 ComputeLighting(Light gLights[MaxLights], Material mat,
                       float3 pos, float3 normal, float3 toEye,
                       float3 shadowFactor)
{
    float3 result = 0.0f;
    int i = 0;
#if (NUM_DIR_LIGHTS > 0)
    for (i = 0; i < NUM_DIR_LIGHTS; ++i)
    {
        result += shadowFactor[i] * ComputeDirectionalLight(gLights[i], mat, normal, toEye);
    }
#endif
    
#if (NUM_POINT_LIGHTS > 0)
    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
    {
        result += ComputePointLight(gLights[i], mat, pos, normal, toEye);

    }
#endif

#if (NUM_SPOT_LIGHTS > 0)
    for(i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
    {
        result += ComputeSpotLight(gLights[i], mat, pos, normal, toEye);
    }
#endif 

    return float4(result, 0.0f);
}

//Lambert's Cosine Law  
//E2 = E1(max(dot(n,L),0);
//L lightDir
//n normal
//E1 Radiant flux  while L == n

//diffuse albedo
//diffuse reflectance
// Cd = dot(max(dot(L,n),0),(BL*md))

//ambient light
//Ca  = AL * md

//speculaar reflection
//fresnel effect
// reflect  Rf 
// refract  (1 - Rf)
// index of refraction

// Rf(rad) = Rf(0') + (1-Rf(0'))pow((1-cos(Rad)),5); 

//roughness
//halfway vector 
// h = normalize(L+v);
// rad between  h and v
// p(rad) = cosm(rad) = cosm(n*h);
// spec(rad) = m + 8 /8 * p(rad) = m+8/8*pow((n*h),m);


//Cs = max(dot(n,L),0)*BL*
//      Rf(ƒ¿h)m+8/8*pow((n*h),m)

//lit Color = Ca + Cd + Cs
//