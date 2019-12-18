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

//VS stage
cbuffer ShadowCB : register(b1)
{
    matrix s_View;
    matrix s_Proj;
    matrix s_Ortho;
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
    //matrix lightView = mul(worldMatrix, s_View);
    //matrix lightViewProj = mul(lightView, s_Ortho);
    vOut.position = mul(float4(vIn.position, 1.0f), worldMatrix);
    vOut.position = mul(vOut.position, s_View);
    vOut.position = mul(vOut.position, s_Proj);
#endif

    return vOut;
}