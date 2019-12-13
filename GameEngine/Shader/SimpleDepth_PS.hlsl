float4 main(float4 pos : Position) : SV_TARGET
{
    float depth = pos.z;
    return (depth, depth, depth, 1.0f);       
}