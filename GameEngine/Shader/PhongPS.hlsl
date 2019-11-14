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
#include "LightVectorData.hlsli"
#include "LightOptions.hlsli"

//cbuffer DirectionLightCBuf : register(b2)
//{
//    float4 ambient;
//    float4 diffuse;
//    float4 specular;
//    float3 direction;
//}

cbuffer ObjectCBuf 
    :register(b4)
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D tex;

SamplerState splr;


float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord) : SV_Target
{
    //renormalize interpolatednormal
    viewNormal = normalize(viewNormal);

    //fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
	// specular
    const float3 specular = Speculate(diffuseColor, diffuseIntensity, viewNormal, lv.vToL, viewFragPos, att, specularPower);
	// final color
    float4 finalColor = 1.0f;
    float4 texColor = tex.Sample(splr, tc);
    clip(texColor.a - 0.1f);
    finalColor.rgb = texColor.rgb * saturate(ambient + diffuse) + specular;
    finalColor.a = texColor.a;

	// final color
    return finalColor;
    //return float4(saturate(diffuse + ambient + specular), 1.0f) * tex.Sample(splr,tc);
}