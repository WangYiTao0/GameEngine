#include "CommonCbuf.hlsli"

struct VS_IN
{
    float4 position : Position;
};

struct VS_OUT
{
    float4 position : SV_Position;
    float4 depthPosition : TEXCOORD0;
};

VS_OUT main(VS_IN vIn)
{
    VS_OUT vOut;

    // Change the position vector to be 4 units for proper matrix calculations.
    vIn.position.w = 1.0f;

	// Calculate the position of the vertex against the world,
    //view, and projection matrices.
    vOut.position = mul(vIn.position, worldMatrix);
    vOut.position = mul(vOut.position, viewMatrix);
    vOut.position = mul(vOut.position, projMatrix);

	// Store the position value in a second vIn value for depth value calculations.
    vOut.depthPosition = vOut.position;
	
    return vOut;
}