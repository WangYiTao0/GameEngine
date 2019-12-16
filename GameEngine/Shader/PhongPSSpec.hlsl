#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonCbuf.hlsli"

struct PS_pIn
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float3 worldNormal : Normal;
    float2 texcoord : Texcoord;
};

//cbuffer ObjectCBuf : register(b2)
//{
//    float3 diff;
//    float shininess;
//    float3 spec;
//    float padding;
//};

cbuffer ObjectCBuf : register(b2)
{
    float specularPower;
    bool hasGloss;
    float specularMapWeight;
};

Texture2D diffTex : register(t0);
Texture2D specTex : register(t1);


SamplerState sample0 : register(s0);

float4 main(PS_pIn pIn) : SV_Target
{
    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);


    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

    float shadowFactor[MaxLights];
    for (int i = 0; i < MaxLights; i++)
    {
        shadowFactor[i] = 1.0f;
    }

    float3 texDiff = diffTex.Sample(sample0, pIn.texcoord).rgb;
      // specular parameters
    float specularPowerLoaded = specularPower;
    const float4 specularSample = specTex.Sample(sample0, pIn.texcoord);
    const float3 specularReflectionColor = specularSample.rgb * specularMapWeight;
    if (hasGloss)
    {
        specularPowerLoaded = pow(2.0f, specularSample.a * 13.0f);
    }

    Material mat = { texDiff, specularReflectionColor, specularPowerLoaded };
    float4 finalColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);

    // final color
    return finalColor;
}