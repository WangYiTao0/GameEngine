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
#include "CommonPSOption.hlsli"

struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 viewPixelPos : Position;
    float3 viewNormal : Normal;
};

cbuffer ObjectCBuf : register(b2)
{
    float4 materialColor;
    float4 specularColor;
    float specularPower;
};


float4 main(PS_INPUT input) : SV_Target
{
    // normalize the mesh normal
    input.viewNormal = normalize(input.viewNormal);
	// fragment to light vector data
    float3 viewLightPos = mul(float4(worldLightPos, 1.0f), view);
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewPixelPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
    // specular
    const float3 specular = Speculate(
        specularColor.rgb, 1.0f, input.viewNormal,
        lv.vToL, input.viewPixelPos, att, specularPower
    );
	// final color
    return float4(saturate((diffuse + ambient) * materialColor.rgb + specular), 1.0f);
}