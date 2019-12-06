#include "CommonPSOption.hlsli"

struct VS_IN
{
    float3 posL : Position;
};

struct VS_OUT
{   
    float3 posL : POSITION;
    float4 posH : SV_Position;
 
};

VS_OUT main(VS_IN vIn)
{
    VS_OUT vOut;
    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView, projMatrix);
    float4 posH = mul(float4(vIn.posL, 1.0f), modelViewprojMatrix);
    //z = w
    vOut.posH = posH.xyww;
    vOut.posL = vIn.posL;
    return vOut;
}
