#include "CommonPSOption.hlsli"

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : Texcoord;
};

Texture2D objTexture : TEXTURE : register(t0);


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 sampleColor = objTexture.Sample(sample0, input.inTexCoord);
    return sampleColor;
}