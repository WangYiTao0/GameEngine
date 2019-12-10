#define MaxLights 16

// Defaults for number of lights.
#ifndef NUM_DIR_LIGHTS
    #define NUM_DIR_LIGHTS 0
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

    float3 diffColor; //  direct spot point 
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
    float3 diff;
    float shininess;
    float3 spec;
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

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for directional lights.
//---------------------------------------------------------------------------------------
float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{
    // The light vector aims opposite the direction the light rays travel.
    float3 lightDir = normalize(-L.direction);

    // Scale light down by Lambert's cosine law.
    //diffuse shadding
    float diff = max(dot(normal, lightDir), 0.0f);

    //specular shading
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(toEye, reflectDir), 0.0f), mat.shininess);
    //combine results
    float3 ambient = L.ambient * mat.diff;
    float3 diffuse = L.diffColor * L.diffuseIntensity * diff * mat.diff;
    float3 specular = L.specular  * spec * mat.spec;

    return saturate(ambient + diffuse) + specular;
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for point lights.
//---------------------------------------------------------------------------------------
float3 ComputePointLight(Light L, Material mat, float3 worldPos, float3 worldNormal, float3 toEye)
{
    // The vector from the surface to the light.
    float3 lightDir = normalize(L.position - worldPos);

    // diffuse shading
    float diff = max(dot(worldNormal, lightDir), 0.0);
    // specular shading
    
    
    float spec = 0.0;
    //blinn
    float3 halfwayDir = normalize(lightDir + toEye);
    spec = pow(max(dot(worldNormal, halfwayDir), 0.0f), mat.shininess);
    //phongshader
    //float3 reflectDir = reflect(-lightDir, worldNormal);
    //v = i - 2 * n * dot(i n) 
    //float3 reflectDir = reflect(-lightDir, worldNormal);
    //spec = pow(max(dot(toEye, reflectDir), 0.0), mat.shinness);
    // attenuation
    float distance = length(L.position - worldPos);
    float attenuation = CalcAttenuation(L.attConst, L.attLin, L.attQuad, distance);
    // combine results
    float3 ambient = L.ambient * mat.diff;
    float3 diffuse = L.diffColor * L.diffuseIntensity * diff * mat.diff;
    float3 specular = L.specular * spec * mat.spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return saturate(ambient + diffuse) + specular;
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for spot lights.
//---------------------------------------------------------------------------------------
float3 ComputeSpotLight(Light L, Material mat, float3 worldPos, float3 normal, float3 toEye)
{
    float3 lightDir = normalize(L.position - worldPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading

    //blinn phong
    float3 halfwayDir = normalize(lightDir + toEye);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mat.shininess);
    //phong shader
    //float3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(toEye, reflectDir), 0.0), mat.shininess);
    // attenuation
    float distance = length(L.position - worldPos);
    float attenuation = CalcAttenuation(L.attConst, L.attLin, L.attQuad, distance);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-L.direction));
    float epsilon = L.cutOff - L.outerCutOff;
    float intensity = clamp((theta - L.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    float3 ambient = L.ambient * mat.diff;
    float3 diffuse = L.diffColor * L.diffuseIntensity * diff * mat.diff;
    float3 specular = L.specular * spec * mat.spec;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return saturate(ambient + diffuse) + specular;
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

