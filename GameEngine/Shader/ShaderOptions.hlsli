
struct LightVectorData
{
    float3 vToL; // vector to light
    float3 dirToL; // dir to light
    float distToL; //distance to light
};


cbuffer PointLightCBuf : register(b1)
{
    float3 worldLightPos;
    float intensity;
    float3 ambient;
    float attConst;
    float3 diffuse;
    float attLin;
    float3 specular;
    float attQuad;
  
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 fragPos)
{
    LightVectorData lv;
    lv.vToL = lightPos - fragPos;
    lv.distToL = length(lv.vToL);
    lv.dirToL = lv.vToL / lv.distToL;

    lv.dirToL.x = lv.vToL.x / lv.distToL.x;
    lv.dirToL.y = lv.vToL.y / lv.distToL.x;
    lv.dirToL.z = lv.vToL.z / lv.distToL.x;
    
    return lv;
}

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

float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
    float dist = distFragToL;
    float attenuate = 1.0f / (attConst + attLin * dist + attQuad * (dist * dist));
    return attenuate;
}

float3 Diffuse(
    uniform float3 diffuseColor,
    uniform float diffuseIntensity,
    const in float att,
    const in float3 viewDirFragToL,
    const in float3 viewNormal)
{
    float a = att;

    return diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirFragToL, viewNormal));
}

float3 Speculate(
    const in float3 specularColor,
    uniform float specularIntensity,
    const in float3 viewNormal,
    const in float3 viewFragToL,
    const in float3 viewPos,
    const in float att,
    const in float specularPower)
{
    // calculate reflected light vector
    const float3 w = viewNormal * dot(viewFragToL, viewNormal);
    const float3 r = normalize(w * 2.0f - viewFragToL);
    // vector from camera to fragment (in view space)
    const float3 viewCamToFrag = normalize(viewPos);
    // calculate specular component color based on angle between
    // viewing vector and reflection vector, narrow with power function
    return att * specularColor * specularIntensity * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPower);
}