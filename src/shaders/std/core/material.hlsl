#include "std/objects/fragment"
#include "std/objects/material"

float GetAperture(float Roughness)
{
    return max(0.01, tan(Roughness * 3.141596 * 0.05));
}
float GetRoughnessMip(Fragment Frag, Material Mat, in float MaxLevels)
{
    return MaxLevels * (Mat.Roughness.x + Frag.Roughness * Mat.Roughness.y);
}
float GetRoughness(Fragment Frag, Material Mat)
{
    return Mat.Roughness.x + Frag.Roughness * Mat.Roughness.y;
}
float3 GetMetallic(Fragment Frag, Material Mat)
{
    return Mat.Metallic.xyz + Frag.Metallic * Mat.Metallic.w;
}
float GetOcclusion(Fragment Frag, Material Mat)
{
    return Mat.Occlusion.x + Frag.Occlusion * Mat.Occlusion.y;
}
float3 GetEmission(Fragment Frag, Material Mat)
{
    return (Mat.Emission.xyz + Frag.Emission) * Mat.Emission.w;
}
float3 GetSurface(Fragment Frag, Material Mat)
{
    return Mat.Emission.xyz + Frag.Emission;
}