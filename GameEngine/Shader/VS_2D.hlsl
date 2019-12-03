#include "CommonPSOption.hlsli"

struct VS_INPUT
{
    float3 inPos : Position;
    float2 inTexCoord : Texcoord;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : Texcoord;
};

VS_OUTPUT main(VS_INPUT vIn)
{
    matrix wvp = mul(worldMatrix2D, orthoMatrix);

    VS_OUTPUT output;
    float4 posW = mul(float4(vIn.inPos, 1.0f), worldMatrix);
    output.outPosition = mul(posW, wvp);
    output.outTexCoord = vIn.inTexCoord;
    return output;
}