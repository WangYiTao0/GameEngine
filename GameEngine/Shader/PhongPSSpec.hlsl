#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonPSOption.hlsli"

struct PS_pIn
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float3 worldNormal : Normal;
    float2 texcoord : Texcoord;
};

cbuffer ObjectCBuf : register(b2)
{
    float3 diff;
    float shininess;
    float3 spec;
    float specPower;
};

Texture2D diffTex : register(t0);
Texture2D specTex : register(t1);



float4 main(PS_pIn pIn) : SV_Target
{
    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);

    //// specular parameters
    //float specularPower = specularPowerConst;
    //const float4 specularSample = spec.Sample(sample0, pIn.texcoord);
    //const float3 specularReflectionColor = specularSample.rgb * specularMapWeight;
    //if (hasGloss)
    //{
    //    specularPower = pow(2.0f, specularSample.a * 13.0f);
    //}

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

    float3 shadowFactor = 1.0f;

    float3 texDiff = diffTex.Sample(sample0, pIn.texcoord).rgb;
    float3 texSpec = specTex.Sample(sample0, pIn.texcoord).rgb;
    Material mat = { texDiff, shininess, texSpec, specPower };
    float4 finalColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);

    // final color
    return finalColor;
}