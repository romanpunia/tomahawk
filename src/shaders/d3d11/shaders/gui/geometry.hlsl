#include "sdk/layouts/gui"
#include "sdk/channels/immediate"
#include "sdk/buffers/object"

VOutput VS(VInput V)
{
	VOutput Result;
	Result.Position = mul(float4(V.Position.xy, 0.0, 1.0), WorldViewProjection);
	Result.Color = V.Color;
	Result.TexCoord = V.TexCoord;
    Result.UV = Result.Position;

	return Result;
};

float4 PS(VOutput V) : SV_Target
{
    float4 Color = V.Color;
    [branch] if (HasDiffuse > 0)
        Color *= GetDiffuse(V.TexCoord);

    return Color;
};