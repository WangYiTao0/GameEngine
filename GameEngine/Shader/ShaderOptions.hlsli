
float3 MapNormal(
    const in float3 tan,
    const in float3 bitan,
    const in float3 normal,
    const in float2 tc,
    uniform Texture2D nmap,
    uniform SamplerState splr)
{
    // build the tranform (rotation) into same space as tan/bitan/normal (target space)
    const float3x3 tanToTarget = float3x3(tan, bitan, normal);

    // sample and unpack the normal from texture into target space   
    const float3 normalSample = nmap.Sample(splr, tc).rgb;
    const float3 tanNormal = normalSample * 2.0f - 1.0f;

    const float3 finalN = normalize(mul(tanNormal, tanToTarget));

    // bring normal from tanspace into target space
    return finalN;
}

