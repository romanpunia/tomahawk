#include "std/layouts/skin"
#include "std/channels/gvoxelizer"
#include "std/buffers/object"
#include "std/buffers/animation"

VOutput vs_main(VInput V)
{
    VOutput Result = (VOutput)0;
	Result.TexCoord = V.TexCoord * ob_TexCoord.xy;

    float4 Position = float4(V.Position, 1.0);
	[branch] if (ab_Animated > 0)
	{
		matrix Offset =
			mul(ab_Offsets[(int)V.Index.x], V.Bias.x) +
			mul(ab_Offsets[(int)V.Index.y], V.Bias.y) +
			mul(ab_Offsets[(int)V.Index.z], V.Bias.z) +
			mul(ab_Offsets[(int)V.Index.w], V.Bias.w);

        Position = mul(float4(V.Position, 1.0), Offset);
		Result.Position = Result.UV = GetVoxel(mul(Position, ob_World));
		Result.Normal = normalize(mul(mul(float4(V.Normal, 0), Offset).xyz, (float3x3)ob_World));
		Result.Tangent = normalize(mul(mul(float4(V.Tangent, 0), Offset).xyz, (float3x3)ob_World));
		Result.Bitangent = normalize(mul(mul(float4(V.Bitangent, 0), Offset).xyz, (float3x3)ob_World));   
    }
	else
	{
		Result.Position = Result.UV = GetVoxel(mul(Position, ob_World));
		Result.Normal = normalize(mul(V.Normal, (float3x3)ob_World));
		Result.Tangent = normalize(mul(V.Tangent, (float3x3)ob_World));
		Result.Bitangent = normalize(mul(V.Bitangent, (float3x3)ob_World));
	}

	return Result;
}

[maxvertexcount(3)]
void gs_main(triangle VOutput V[3], inout TriangleStream<VOutput> Stream)
{
	uint Dominant = GetDominant(V[0].Normal, V[1].Normal, V[2].Normal);
	[unroll] for (uint i = 0; i < 3; ++i)
        V[i].Position = GetVoxelSpace(V[i].Position, Dominant);
             
    ConvervativeRasterize(V[0].Position, V[1].Position, V[2].Position);
	[unroll] for (uint j = 0; j < 3; ++j)
        Stream.Append(V[j]);
}

Lumina ps_main(VOutput V)
{
	float4 Color = float4(Materials[ob_Mid].Diffuse, 1.0);
	[branch] if (ob_Diffuse > 0)
		Color *= GetDiffuse(V.TexCoord);

	float3 Normal = V.Normal;
	[branch] if (ob_Normal > 0)
        Normal = GetNormal(V.TexCoord, V.Normal, V.Tangent, V.Bitangent);
    
    return Compose(V.TexCoord, Color, Normal, V.UV.xyz, ob_Mid);
};