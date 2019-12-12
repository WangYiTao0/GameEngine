#include "CommonCbuf.hlsli"

float4 main(float3 pos : Position) : SV_Position
{
    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView, projMatrix);

    return mul(float4(pos, 1.0f), modelViewprojMatrix);
}