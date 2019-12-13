#include "CommonCbuf.hlsli"

float4 main( float4 pos : Position ) : SV_POSITION
{
    pos.z = 1.0f;
    matrix wvp = mul(mul(worldMatrix, s_View), s_Proj);

    float4 posSV = mul(pos, wvp);

	return pos;
}