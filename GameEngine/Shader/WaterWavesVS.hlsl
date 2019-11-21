#include "CommonPSOption.hlsli"


struct VS_IN
{
    float3 pos : Position;
    float4 Color : Color;
};


struct VS_OUT
{
    float3 viewPos : Position;
    float4 Color : Color;

    float4 pos : SV_Position;
};

VS_OUT main(VS_IN vs_in)
{
    VS_OUT vs_out;
    matrix modelView = mul(world, view);
    matrix modelViewProj = mul(modelView, proj);
    //Camera view Positon
    vs_out.viewPos = (float3) mul(float4(vs_in.pos, 1.0f), modelView);

    vs_out.viewPos.y = sin(vs_out.viewPos.x);

    vs_out.Color = vs_in.Color;

    //world pos
    vs_out.pos = mul(float4(vs_in.pos, 1.0f), modelViewProj);

    return vs_out;

}