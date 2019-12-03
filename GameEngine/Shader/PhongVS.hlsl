#include "CommonPSOption.hlsli"

struct VS_INPUT
{
    float3 pos : Position;
    float3 normal : Normal;
    float2 texcoord : Texcoord;
};

struct VS_OUTPUT
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float2 tc : Texcoord;
    //SV_Position describes the pixel location.
    float4 pos : SV_Position;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT vso;

    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView , projMatrix);

    //Camera view Positon
    vso.viewPos = (float3) mul(float4(input.pos, 1.0f), modelView);
    //Camera view Normal
    vso.viewNormal = mul(input.normal, (float3x3) modelView);
    //worldMatrix pos
    vso.pos = mul(float4(input.pos, 1.0f), modelViewprojMatrix);
    vso.tc = input.texcoord;
    return vso;
}