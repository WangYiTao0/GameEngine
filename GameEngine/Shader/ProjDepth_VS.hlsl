#include "CommonCbuf.hlsli"

struct ObjectMatrices
{
    matrix wvp;
};


cbuffer perModel
{
#ifdef INSTANCED
	ObjectMatrices ObjMats[INSTANCE_COUNT];
#else
    ObjectMatrices ObjMats;
#endif
}


struct VS_IN
{
    float3 position : Position;
#ifdef INSTANCED
	uint instanceID : SV_InstanceID;
#endif
};

struct VS_OUT
{
    float4 position : SV_Position;
};

VS_OUT main(VS_IN vIn) : SV_POSITION
{
    VS_OUT vOut;
    

#ifdef INSTANCED
	vOut.position = mul(ObjMats[In.instanceID].wvp, float4(vIn.position, 1));
#else
    matrix lightView = mul(worldMatrix, s_View);
    matrix lightViewProj = mul(lightView, s_Proj);
    vOut.position = mul(float4(vIn.position, 1.0f), lightViewProj);
#endif

    return vOut;
}