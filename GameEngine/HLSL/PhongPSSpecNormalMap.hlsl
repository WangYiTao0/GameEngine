cbuffer LightCBuf
{
    float3 viewLightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};

Texture2D tex;
Texture2D spec;
Texture2D nmap;

SamplerState splr;

float3 MapNormalViewSpace(const float3 tan, const float3 bitan, const float3 viewNormal, const float2 tc, Texture2D nmap, SamplerState sptr)
{
    // build the tranform (rotation) into tangent space
    const float3x3 tanToView = float3x3(
            normalize(tan),
            normalize(bitan),
            normalize(viewNormal)
        );
    //sample and unpack the normal from texture into tangent space
    const float3 normalSample = nmap.Sample(splr, tc).xyz;
    const float3 tanNormal = normalSample * 2.0f - 1.0f;
    //bring normal from tanspace into view space

    return normalize(mul(tanNormal, tanToView));

}


float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord) : SV_Target
{
    // sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
        viewNormal = MapNormalViewSpace(tan, bitan, viewNormal, tc, nmap, splr);
    }
	// fragment to light vector data
    const float3 viewFragToL = viewLightPos - viewPos;
    const float distFragToL = length(viewFragToL);
    const float3 viewDirFragToL = viewFragToL / distFragToL;
	// attenuation
    const float att = 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirFragToL, viewNormal));
	// reflected light vector
    const float3 w = viewNormal * dot(viewFragToL, viewNormal);
    const float3 r = w * 2.0f - viewFragToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if (specularMapEnabled)
    {
        const float4 specularSample = spec.Sample(splr, tc);
        specularReflectionColor = specularSample.rgb * specularMapWeight;
        float specularPower;

        if (hasGloss)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
    }
    else
    {
        specularPower = specularColor;
    }

    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular * specularReflectionColor), 1.0f);
}