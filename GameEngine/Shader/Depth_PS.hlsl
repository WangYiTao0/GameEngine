
struct PS_IN
{
    float4 position : SV_Position;
    float4 depthPosition : TEXCOORD0;
};

float4 main(PS_IN pIn) : SV_TARGET
{
    float depthValue;
    float4 color;

	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    depthValue = pIn.depthPosition.z / pIn.depthPosition.w;

    color = float4(depthValue, depthValue, depthValue, 1.0f);

    return color;
}