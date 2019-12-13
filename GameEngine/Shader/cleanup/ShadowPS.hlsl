#include "LightingUtil.hlsli"
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);
SamplerState WrapSampleType : register(s0);
SamplerState ClampSampleType : register(s1);

struct PS_IN
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
    float2 Tex : TEXCOORD0;
    float3 W_Normal : NORMAL;
    float3 Pos_W : NORMAL1;
 
};

float4 main(PS_IN pIn) : SV_Target
{
    float4 TexColor;
    float ShadowMapDepth;
    float DiffuseFactor;
    float4 DiffuseLight;
    float2 ShadowTex;
    float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
    float Depth;
    float bias;
 
    bias = 0.001f;
 
    TexColor = shaderTexture.Sample(WrapSampleType, pIn.Tex);
 
    color = float4(gLights[1].ambient, 1.0f);
 
    ShadowTex.x = (pIn.ProjPos.x / pIn.ProjPos.w) * 0.5f + 0.5f;
    ShadowTex.y = (pIn.ProjPos.y / pIn.ProjPos.w) * (-0.5f) + 0.5f;

    if (saturate(ShadowTex.x) == ShadowTex.x && saturate(ShadowTex.y) == ShadowTex.y)
    {
        ShadowMapDepth = depthMapTexture.Sample(ClampSampleType, ShadowTex).r;
 
        Depth = pIn.ProjPos.z / pIn.ProjPos.w;
 
        ShadowMapDepth = ShadowMapDepth + bias;

        if (ShadowMapDepth >= Depth)
        {
			
            float3 DiffuseDir = pIn.Pos_W - gLights[0].position;
 
            float distance = length(DiffuseDir);
 
            float atten1 = 0.5f;
            float atten2 = 0.1f;
            float atten3 = 0.0f;
            float LightIntensity = 1.0f / (atten1 + atten2 * distance + atten3 * distance * distance);
 
            float3 InvseDiffuseDir = -DiffuseDir;
 
            DiffuseFactor = saturate(dot(InvseDiffuseDir, pIn.W_Normal));
		
            DiffuseLight = DiffuseFactor * float4(gLights[1].diffColor, 1.0f) * LightIntensity;
 
            color += DiffuseLight;
		
            color = saturate(color);
 
        }
    }
	
    color = color * TexColor;
 
    return color;
}