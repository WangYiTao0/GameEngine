#define MaxLight 16

struct LightVectorData
{
    float3 vToL; // vector to light
    float3 dirToL; // dir to light
    float distToL; //distance to light
};


cbuffer PointLightCBuf : register(b1)
{
    float3 worldLightPos;
    float diffuseIntensity;
    float3 ambient;
    float attConst;
    float3 diffuseColor;
    float attLin;
    float3 cameraPos;
    float attQuad;

    //float padding;
};

struct CommonMaterial
{
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float3 specularColor;
    float specularIntensity;
    float3 reflect;

};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 worldPos)
{
    LightVectorData lv;
    lv.vToL = lightPos - worldPos;
    lv.distToL = length(lv.vToL);
    lv.dirToL = lv.vToL / lv.distToL;
    return lv;
}



float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
    float dist = distFragToL;
    float attenuate = 1.0f / (attConst + attLin * dist + attQuad * (dist * dist));
    return attenuate;
}

float3 Diffuse(
    uniform float3 diffuse,
    uniform float diffuseIntensity,
    const in float att,
    const in float3 worldDirFragToL,
    const in float3 worldNormal)
{
    return diffuse * diffuseIntensity * att * max(0.0f, dot(worldDirFragToL, worldNormal));
}

float3 Speculate(
    const in float3 specularColor,
    uniform float specularIntensity,
    const in float3 worldNormal,
    const in float3 worldFragToL,
    const in float3 worldPos,
    const in float3 cameraPos,
    const in float att,
    const in float specularPower)
{
    // calculate reflected light vector
    const float3 w = worldNormal * dot(worldFragToL, worldNormal);
    const float3 r = normalize(w * 2.0f - worldFragToL);
    // vector from camera to fragment (in view space)
    const float3 viewCamToFrag = normalize(worldPos-cameraPos);
    // calculate specular component color based on angle between
    // viewing vector and reflection vector, narrow with power function
    return att * specularColor * specularIntensity * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPower);
}


//Lambert's Cosine Law  
//E2 = E1(max(dot(n,L),0);
//L lightDir
//n normal
//E1 Radiant flux  while L == n

//diffuse albedo
//diffuse reflectance
// Cd = dot(max(dot(L,n),0),(BL*md))

//ambient light
//Ca  = AL * md

//speculaar reflection
//fresnel effect
// reflect  Rf 
// refract  (1 - Rf)
// index of refraction

// Rf(rad) = Rf(0') + (1-Rf(0'))pow((1-cos(Rad)),5); 

//roughness
//halfway vector 
// h = normalize(L+v);
// rad between  h and v
// p(rad) = cosm(rad) = cosm(n*h);
// spec(rad) = m + 8 /8 * p(rad) = m+8/8*pow((n*h),m);


//Cs = max(dot(n,L),0)*BL*
//      Rf(ƒ¿h)m+8/8*pow((n*h),m)

//lit Color = Ca + Cd + Cs
//