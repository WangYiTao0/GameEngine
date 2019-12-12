#include "CommonCbuf.hlsli"
#include "LightingUtil.hlsli"


struct VS_IN
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD0; //
    float3 Normal : NORMAL;
};


struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION; //light view
    float2 Tex : TEXCOORD0;
    float3 W_Normal : NORMAL; //world
    float3 Pos_W : NORMAL1; //w
 
};

VS_OUT main(VS_IN vIn)
{
    VS_OUT vOut;

    vOut.Pos = mul(float4(vIn.Pos, 1.0f), worldMatrix);
    vOut.Pos = mul(vOut.Pos, viewMatrix);
    vOut.Pos = mul(vOut.Pos, projMatrix);
 

    vOut.W_Normal = mul(vIn.Normal, (float3x3) worldInvTranspose);
 
    vOut.W_Normal = normalize(vOut.W_Normal);
 
    vOut.Tex = vIn.Tex;
 
    vOut.ProjPos = mul(float4(vIn.Pos, 1.0f), worldMatrix);
    vOut.ProjPos = mul(vOut.ProjPos, s_View);
    vOut.ProjPos = mul(vOut.ProjPos, s_Proj);
 
    vOut.Pos_W = (float3) mul(float4(vIn.Pos, 1.0f), worldMatrix);

    return vOut;

}