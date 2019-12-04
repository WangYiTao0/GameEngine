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



cbuffer ObjectCBuf : register(b2)
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 viewPixelPos : Position;
    float3 viewNormal : Normal;
    float2 texcoord : Texcoord;
};

Texture2D tex : register(t0);


float4 main(PS_INPUT input) : SV_Target
{

    //renormalize interpolatednormal
    input.viewNormal = normalize(input.viewNormal);

    float3 viewLightPos = mul(float4(worldLightPos, 1.0f), viewMatrix);

    //fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewPixelPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diff = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
	// specular
    const float3 specular = Speculate(diffuseColor, specularIntensity, input.viewNormal, lv.vToL, input.viewPixelPos, cameraPos, att, specularPower);
	// final color
    float4 finalColor = 1.0f;
    float4 texColor = tex.Sample(sample0, input.texcoord);
    clip(texColor.a - 0.1f);
    finalColor.rgb = texColor.rgb * saturate(ambient + diff) + specular;
    finalColor.a = texColor.a;

	// final color
    return finalColor;
    //return float4(saturate(diffuse + ambient + specular), 1.0f) * tex.Sample(sample0,tc);
}