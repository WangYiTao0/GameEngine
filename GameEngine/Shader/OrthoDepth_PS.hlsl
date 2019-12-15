#include "CommonCbuf.hlsli"

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : Texcoord;
};

Texture2D depthTex : TEXTURE : register(t0);

SamplerState sample0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 sampleColor = (depthTex.Sample(sample0, input.inTexCoord).www,1.0f);
    return sampleColor;
}