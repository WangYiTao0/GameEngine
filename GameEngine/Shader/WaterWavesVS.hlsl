#include "CommonPSOption.hlsli"

cbuffer vsWaterConstant :register (b1)
{
    float3 pos;
};

struct VS_IN
{
    float3 pos : Position;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float3 biTangent : Bitangent;
    float2 texcoord : Texcoord;
};


struct VS_OUT
{
    float3 viewPos : Position;


    float4 pos : SV_Position;
};

VS_OUT main(VS_IN vIn)
{
    VS_OUT vOut;
    matrix modelView = mul(world, view);
    matrix modelViewProj = mul(modelView, proj);
    //Camera view Positon

    vIn.pos += pos;

    vOut.viewPos = (float3) mul(float4(vIn.pos, 1.0f), modelView);

    //world pos
    vOut.pos = mul(float4(vIn.pos, 1.0f), modelViewProj);

    return vOut;

}