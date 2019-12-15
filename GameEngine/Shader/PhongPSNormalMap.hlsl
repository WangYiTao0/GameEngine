#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonCbuf.hlsli"
#include "ShadingMath.hlsli"


struct PS_pIn
{
    //SV_Position describes the pixel location.
    float3 worldPos      : Position;
    float4 lightSpacePos : POSITION1;
    float3 worldNormal   : Normal;
    float3 worldTan      : Tangent;
    float3 worldBitan    : Bitangent;
    float2 texcoord      : Texcoord;
};

cbuffer ObjectCBuf : register(b2)
{
    float3 diff;
    float shininess;
    float3 spec;
    float padding;
};

Texture2D diffTex : register(t0);
Texture2D nmapTex : register(t2);
Texture2D shadowTex : register(t3);


SamplerState sampleWrap : register(s0);
SamplerState sampleClamp : register(s1);
float4 main(PS_pIn pIn) : SV_Target
{
    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);
    pIn.worldTan = normalize(pIn.worldTan);
    pIn.worldBitan = normalize(pIn.worldBitan);

    // replace normal with mapped if normal mapping enabled
    pIn.worldNormal = MapNormal(pIn.worldTan, pIn.worldBitan, pIn.worldNormal, pIn.texcoord, nmapTex, sampleWrap);
    
    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);



   float shadowFactor[MaxLights];
    for (int i = 0; i < MaxLights; i++)
    {
        //ShadowCalculation(pIn.lightSpacePos, shadowTex, sample0);
        shadowFactor[i] = 1.0f;
    }

    shadowFactor[0] = 1.0f - ShadowCalculation(pIn.lightSpacePos, shadowTex, sampleClamp, gLights[0].position, pIn.worldPos, pIn.worldNormal);

    //vector<float, MaxLights> shadowFactor;

    //using texture material 
    //if doesn't have texture material using common material
    float3 texDiff = diffTex.Sample(sampleWrap, pIn.texcoord).rgb;

    Material mat = { texDiff, shininess, spec };
    float4 finalColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);

    // final color
    return finalColor;
}
