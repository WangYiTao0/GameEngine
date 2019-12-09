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
    //float specularPowerConst;
    //bool hasGloss;
    //float specularMapWeight;
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
};

Texture2D diffTex : register(t0);
Texture2D spec : register(t1);



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