struct LightVectorData
{
    float3 vToL; // vector to light
    float3 dirToL; // dir to light
    float distToL; //distance to light
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 fragPos)
{
    LightVectorData lv;
    lv.vToL = lightPos - fragPos;
    lv.distToL = length(lv.vToL);
    //lv.dirToL = lv.vToL / lv.distToL;
    lv.dirToL = normalize(lv.vToL);
    return lv;
}

