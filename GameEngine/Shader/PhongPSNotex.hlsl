/*Pointlight 
1.vector to light pos  v_to_l = light_pos - pos
2 distance = v_to_light.len();
3 direction = v_to_light /dist;

light Pos
4 Canculate attenuation
  Attenuation = 1 /(Ld*d+Qd+C)
  linear attenuation
  quadradic attnuation
  constant attnuation
5 calculate intensity based on angle of incidence and attenuation
6 add diffuse+ambient, filter by material color, saturate and scale
*/ 
#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonPSOption.hlsli"

struct PS_pIn
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float3 worldNormal : Normal;
};

cbuffer ObjectCBuf : register(b2)
{
    float4 materialColor;
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
};


float4 main(PS_pIn pIn) : SV_Target
{
    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);
    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

  	// Indirect lighting.
    float4 ambient = gAmbientLight * gDiffuseAlbedo;


    float4 finalColor = 1.0f;
   // float4 diffTexColor = diffTex.Sample(sample0, pIn.texcoord);

    const float shininess = 1.0f - gRoughness;
    Material mat = { gDiffuseAlbedo, gFresnelR0, shininess };
    float3 shadowFactor = 1.0f;
    float4 LightColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);

    finalColor = materialColor * (ambient + LightColor);

    // Common convention to take alpha from diffuse material.
    finalColor.a = gDiffuseAlbedo.a;
    // final color
    return finalColor;

}