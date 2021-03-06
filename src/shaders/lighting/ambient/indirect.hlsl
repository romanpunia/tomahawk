#include "std/layouts/shape"
#include "std/channels/raytracer"
#include "std/core/raytracing"
#include "std/core/position"

VOutput vs_main(VInput V)
{
	VOutput Result = (VOutput)0;
	Result.Position = mul(float4(V.Position, 1.0), vxb_WorldViewProj);
	Result.TexCoord = Result.Position;

	return Result;
}

float4 ps_main(VOutput V) : SV_TARGET0
{
	Fragment Frag = GetFragment(GetTexCoord(V.TexCoord));
    [branch] if (Frag.Depth >= 1.0)
        return float4(Frag.Diffuse, 1.0);

    Material Mat = Materials[Frag.Material];
    float R = GetRoughness(Frag, Mat), Shadow;
	float3 M = GetMetallic(Frag, Mat);
    float3 E = GetEmission(Frag, Mat);
    float3 D = normalize(Frag.Position - vb_Position);
    float4 Radiance = GetRadiance(Frag.Position, Frag.Normal, M, Shadow);
    float4 Reflectance = GetSpecular(Frag.Position, Frag.Normal, D, M, R);
    float4 Result = vxb_Intensity * float4(E + Radiance.xyz, Radiance.w) + Reflectance;
    Result.xyz = Frag.Diffuse + Result.xyz * Result.w;

    return float4(Result.xyz * Shadow, Shadow);
};