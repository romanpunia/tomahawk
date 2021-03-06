#include "std/layouts/skin"
#include "std/channels/depth"
#include "std/buffers/object"
#include "std/buffers/animation"

VOutputLinear vs_main(VInput V)
{
	VOutputLinear Result = (VOutputLinear)0;
	Result.Normal = normalize(mul(V.Normal, (float3x3)ob_World));
	Result.TexCoord = V.TexCoord * ob_TexCoord.xy;

	[branch] if (ab_Animated > 0)
	{
		matrix Offset =
			mul(ab_Offsets[(int)V.Index.x], V.Bias.x) +
			mul(ab_Offsets[(int)V.Index.y], V.Bias.y) +
			mul(ab_Offsets[(int)V.Index.z], V.Bias.z) +
			mul(ab_Offsets[(int)V.Index.w], V.Bias.w);

		Result.Position = Result.UV = mul(mul(float4(V.Position, 1.0), Offset), ob_WorldViewProj);
	}
	else
		Result.Position = Result.UV = mul(float4(V.Position, 1.0), ob_WorldViewProj);

	return Result;
}