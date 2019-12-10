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
Texture2D nmapTex : register(t2);


float4 main(PS_pIn pIn) : SV_Target
{
    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView, projMatrix);

    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);
	// sample normal from map if normal mapping enabled

     // sample and unpack normal data
    const float3 normalSample = nmapTex.Sample(sample0, pIn.texcoord).xyz;
     const float3 objectNormal = normalSample * 2.0f - 1.0f;
     // bring normal from object space into view space
     pIn.worldNormal = normalize(mul(objectNormal, (float3x3) modelView));

   // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

    float3 shadowFactor = 1.0f;

    float3 texDiff = diffTex.Sample(sample0, pIn.texcoord).rgb;

    Material mat = { texDiff, shininess, spec, specPower };
    float4 finalColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);

    // final color
    return finalColor;
}