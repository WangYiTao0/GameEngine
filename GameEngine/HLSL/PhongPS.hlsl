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


cbuffer LightCBuf :register (b0)
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf:register(b1)
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
};


float4 main(float3 worldPos : Position, float3 normal : Normal) : SV_Target
{
    //fragment to light vector data
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att *
    max(0.0f, dot(dirToL, normal));
    // final color
    /*
    float saturate( float v)
    {
        if( v < 0.0 ) v = 0.0;
        if( v > 1.0 ) v = 1.0;
    }
    */

    // reflected light vector
    const float3 w = normal * dot(vToL, normal);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between 
    //viewing vector and reflection vector, narrow with power
    //function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity *
    pow(
        max(0.0f, 
             dot(normalize(-r),normalize(worldPos)
    )), specularPower);
	

    return float4(saturate((diffuse + ambient + specular) * materialColor), 1.0f);


}