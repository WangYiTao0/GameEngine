#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonPSOption.hlsli"
#include "ShadingMath.hlsli"


struct PS_pIn
{
    //SV_Position describes the pixel location.
    float3 worldPos      : Position;
    float3 worldNormal   : Normal;
    float3 worldTan      : Tangent;
    float3 worldBitan    : Bitangent;
    float2 texcoord      : Texcoord;
};

cbuffer ObjectCBuf :register (b2)
{
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
};

Texture2D diffTex : register(t0);
Texture2D nmap : register(t2);


float4 main(PS_pIn pIn) : SV_Target
{
 

    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);
    pIn.worldTan = normalize(pIn.worldTan);
    pIn.worldBitan = normalize(pIn.worldBitan);

    // replace normal with mapped if normal mapping enabled
    pIn.worldNormal = MapNormal(pIn.worldTan, pIn.worldBitan, pIn.worldNormal, pIn.texcoord, nmap, sample0);
    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

  	// Indirect lighting.
    float4 ambient = gAmbientLight * gDiffuseAlbedo;


    float4 finalColor = 1.0f;
    float4 diffTexColor = diffTex.Sample(sample0, pIn.texcoord);

    const float shininess = 1.0f - gRoughness;
    Material mat = { gDiffuseAlbedo, gFresnelR0, shininess };
    float3 shadowFactor = 1.0f;
    float4 LightColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);

    finalColor = diffTexColor * (ambient + LightColor);

    // Common convention to take alpha from diffuse material.
    finalColor.a = gDiffuseAlbedo.a;
    // final color
    return finalColor;
}