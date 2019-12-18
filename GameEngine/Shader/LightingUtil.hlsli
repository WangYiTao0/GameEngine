#define MaxLights 16
// some manual tuning...
#define SPOTLIGHT_BRIGHTNESS_SCALAR 0.001f
#define SPOTLIGHT_BRIGHTNESS_SCALAR_PHONG 0.00028f
#define POINTLIGHT_BRIGHTNESS_SCALAR_PHONG 0.002f

// Defaults for number of lights.
#ifndef NUM_DIR_LIGHTS
#define NUM_DIR_LIGHTS 1
#endif

#ifndef NUM_POINT_LIGHTS
#define NUM_POINT_LIGHTS 4
#endif

#ifndef NUM_SPOT_LIGHTS
#define NUM_SPOT_LIGHTS 1
#endif

struct Light
{
    float3 position; //spot point
    float attQuad; //point spot

    float3 Color; //  direct spot point 
    float attLin; //point spot

    float3 direction; //direct spot
    float attConst; //point spot

    float3 ambient; //direct spot point 
    float spotPower; //spot

    float cutOff; //spot
    float outerCutOff; //spot
    //	const float brightness = dot(float3(0.2126, 0.7152, 0.0722), color.xyz); // luma conversion
    float brightness;
    float LightPadding;

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
    float3 materialColor; //diffuseColor
    float3 specularColor;
    //float specularIntensity;
    float specularPower;
};




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
    const float3 worldCamToFrag = normalize(worldPos - cameraPos);
    // calculate specular component color based on angle between
    // viewing vector and reflection vector, narrow with power function
    return att * specularColor * specularIntensity * pow(max(0.0f, dot(-r, worldCamToFrag)), specularPower);
}

float ShadowCalculation(float4 fragPosLightSpace, Texture2D shadowMap, SamplerState clampSample,
float3 lightPos, float3 worldPos, float3 worldNormal)
{

    // perform perspective divide
    float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    //[0.0,1.0]  u=0.5*x+0.5;   v=-0.5*y+0.5;
    projCoords.x = projCoords.x * 0.5f + 0.5f;
    projCoords.y = projCoords.y * (-0.5f) + 0.5f;

    if (saturate(projCoords.x) == projCoords.x && saturate(projCoords.y) == projCoords.y)
    {
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        float closestDepth = shadowMap.Sample(clampSample, projCoords.xy).r;
    // get depth of current fragment from light's perspective
        float currentDepth = projCoords.z;

    // Calculate bias (based on depth map resolution and slope)
        float3 normal = normalize(worldNormal);
        float3 lightDir = normalize(lightPos - worldPos);
        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

        // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
        float shadow = 0.0;
        float width, height;
        shadowMap.GetDimensions(width, height);
        float2 texelSize = 1.0 / float2(width, height);
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                float pcfDepth = shadowMap.Sample(clampSample, projCoords.xy + float2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
        if (projCoords.z > 1.0)
            shadow = 0.0;

        return shadow;
    }
    return 0.0f;
}


//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for directional lights.
//---------------------------------------------------------------------------------------
float3 ComputeDirectionalLight(Light L,
Material mat,
float3 normal,
float3 toEye,
float shadowFactor)
{
    // The light vector aims opposite the direction the light rays travel.
    float3 lightDir = normalize(-L.direction);

    // Scale light down by Lambert's cosine law.
    //diffuse shadding
    float diff = max(dot(normal, lightDir), 0.0f);

    //specular shading
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(toEye, reflectDir), 0.0f), mat.specularPower);
    //combine results
    float3 ambient = L.ambient * mat.materialColor;
    float3 diffuse = L.brightness * L.Color * diff * mat.materialColor;
    float3 specular = L.Color * spec * mat.specularColor;

    return ambient + shadowFactor * (diffuse + specular);
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for point lights.
//---------------------------------------------------------------------------------------
float3 ComputePointLight(Light L,
Material mat,
float3 worldPos,
float3 worldNormal,
float3 toEye, float shadowFactor)
{
    // The vector from the surface to the light.
    float3 lightDir = normalize(L.position - worldPos);

    // diffuse shading
    float diff = max(dot(worldNormal, lightDir), 0.0);
    // specular shading
    
    float spec = 0.0;
    //blinn
    float3 halfwayDir = normalize(lightDir + toEye);
    spec = pow(max(dot(worldNormal, halfwayDir), 0.0f), mat.specularPower);
    //phongshader
    //float3 reflectDir = reflect(-lightDir, worldNormal);
    //v = i - 2 * n * dot(i n) 
    //float3 reflectDir = reflect(-lightDir, worldNormal);
    //spec = pow(max(dot(toEye, reflectDir), 0.0), mat.shinness);
    // attenuation
    float distance = length(L.position - worldPos);
    float attenuation = CalcAttenuation(L.attConst, L.attLin, L.attQuad, distance);
    // combine results
    float3 ambient = L.ambient * mat.materialColor;
    float3 diffuse = L.Color * diff * mat.materialColor;
    float3 specular = L.Color * spec * mat.specularColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + 1.0f * (diffuse + specular);
}


//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for spot lights.
//---------------------------------------------------------------------------------------
float3 ComputeSpotLight(Light L, Material mat,
float3 worldPos, float3 normal,
float3 toEye, float shadowFactor)
{
    float3 lightDir = normalize(L.position - worldPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading

    //blinn phong
    float3 halfwayDir = normalize(lightDir + toEye);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mat.specularPower);
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
    float3 ambient = L.ambient * mat.materialColor;
    float3 diffuse = L.Color * diff * mat.materialColor;
    float3 specular = L.Color * spec * mat.specularColor;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return ambient + 1.0f * (diffuse + specular);
}



float4 ComputeLighting(Light gLights[MaxLights], Material mat,
                       float3 pos, float3 normal, float3 toEye,
                       float shadowFactor[MaxLights])
{


    float3 result = 0.0f;
    int i = 0;
#if (NUM_DIR_LIGHTS > 0)
    for (i = 0; i < NUM_DIR_LIGHTS; ++i)
    {
        result += ComputeDirectionalLight(gLights[i], mat, normal, toEye, shadowFactor[i]);
    }
#endif
    
#if (NUM_POINT_LIGHTS > 0)
    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
    {
        result += ComputePointLight(gLights[i], mat, pos, normal, toEye, shadowFactor[i]);

    }
#endif

#if (NUM_SPOT_LIGHTS > 0)
    for(i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
    {
        result += ComputeSpotLight(gLights[i], mat, pos, normal, toEye, shadowFactor[i]);
    }
#endif 

    return float4(result, 0.0f);
}

