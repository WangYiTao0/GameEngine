#include "ShaderOptions.hlsli"
#include "LightVectorData.hlsli"
#include "CommonPSOption.hlsli"

struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 viewPixelPos : Position;
    float3 viewNormal : Normal;
    float2 texcoord : Texcoord;
};

cbuffer ObjectCBuf : register(b2)
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};


Texture2D tex : register(t0);
Texture2D nmap : register(t2);


float4 main(PS_INPUT input) : SV_Target
{
    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView, projMatrix);

    // normalize the mesh normal
    input.viewNormal = normalize(input.viewNormal);
	// sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
        // sample and unpack normal data
        const float3 normalSample = nmap.Sample(sample0, input.texcoord).xyz;
        const float3 objectNormal = normalSample * 2.0f - 1.0f;
        // bring normal from object space into view space
        input.viewNormal = normalize(mul(objectNormal, (float3x3) modelView));
    }
	// fragment to light vector data
    float3 viewLightPos = mul(float4(worldMatrixLightPos, 1.0f), viewMatrix);
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewPixelPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
	// specular
    const float3 specular = Speculate(
        specularIntensity.rrr, 1.0f, input.viewNormal, lv.vToL,
        input.viewPixelPos, att, specularPower
    );

    float4 finalColor = 1.0f;
    float4 texColor = tex.Sample(sample0, input.texcoord);
       //clip(texColor.a - 0.1f);
    clip(texColor.a < 0.1f ? -1 : 1);
    // flip Normal when backface
    if (dot(input.viewNormal, input.viewPixelPos) >= 0.0f)
    {
        input.viewNormal = -input.viewNormal;
    }
    finalColor.rgb = texColor.rgb * saturate(ambient + diffuse) + specular;
    finalColor.a = texColor.a;

	// final color
    return finalColor;
}