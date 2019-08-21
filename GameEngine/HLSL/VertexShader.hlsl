//output twothing using strut

struct VSOut
{	
	float3 color : Color;
	float4 pos : SV_Position;

};

cbuffer cBuf
{
	row_major matrix transform;
};

VSOut main(float2 pos : POSITION,float3 color : Color)
{
	VSOut vso;
	vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f),transform);
	vso.color = color;
	return vso;
}