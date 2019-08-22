cbuffer CBuf
{
	float4 face_colors[6];
};
//SV_PrimitiveID tell the pipeline to generate a primitive id for
//every triangle unique ID
float4 main(uint tid : SV_PrimitiveID) : SV_TARGET
{
	return face_colors[tid / 2];
}