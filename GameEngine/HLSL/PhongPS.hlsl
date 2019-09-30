/*Pointlight 
1.vector to light pos  v_to_l = light_pos - pos
2 distance = v_to_light.len();
3 direction = v_to_light /dist;

light Pos
4 Canculate attenuation
  Attenuation = 1 /(Ld*d+Qd+C)
  linear attenuation
  quadradic attnuation
  constant attnuation
5 calculate intensity based on angle of incidence and attenuation
6 add diffuse+ambient, filter by material color, saturate and scale
*/ 


cbuffer PointLightCBuf :register (b0)
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

//cbuffer DirectionLightCBuf : register(b2)
//{
//    float4 ambient;
//    float4 diffuse;
//    float4 specular;
//    float3 direction;
//}

cbuffer ObjectCBuf:register(b1)
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D tex;

SamplerState splr;


float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord) : SV_Target
{
    //renormalize interpolatednormal
    viewNormal = normalize(viewNormal);

    //fragment to light vector data
    const float3 vToL = lightPos - viewPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, viewNormal));
    // final color
    /*
    float saturate( float v)
    {
        if( v < 0.0 ) v = 0.0;
        if( v > 1.0 ) v = 1.0;
    }
    */

    // reflected light vector
    const float3 w = viewNormal * dot(vToL, viewNormal);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between 
    //viewing vector and reflection vector, narrow with power
    //function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity *
    pow(
        max(0.0f, 
             dot(normalize(-r), normalize(viewPos)
    )), specularPower);
	
    //diffuse * tex.diffuse  specular * tex.specular 
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
    
    //return float4(saturate(diffuse + ambient + specular), 1.0f) * tex.Sample(splr,tc);
}