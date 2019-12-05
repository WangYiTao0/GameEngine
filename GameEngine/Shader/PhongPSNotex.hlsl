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

struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float3 worldNormal : Normal;
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
    input.worldNormal = normalize(input.worldNormal);

    const LightVectorData lv = CalculateLightVectorData(worldLightPos, input.worldPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diff = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.worldNormal);
    // specular
    const float3 specular = Speculate(
        specularColor.rgb, 1.0f, input.worldNormal,
        lv.vToL, input.worldPos, cameraPos, att, specularPower
    );
	// final color
    return float4(saturate((diff + ambient) * materialColor.rgb + specular), 1.0f);
}