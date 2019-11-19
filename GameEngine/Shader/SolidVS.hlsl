#include "Transform.hlsli"
float4 main(float3 pos : Position) : SV_Position
{
    matrix modelView = mul(world, view);
    matrix modelViewProj = mul(modelView, proj);

    return mul(float4(pos, 1.0f), modelViewProj);
}