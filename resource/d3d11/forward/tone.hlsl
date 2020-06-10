#include "standard/space-uv"

cbuffer RenderConstant : register(b3)
{
	float3 BlindVisionR;
	float VignetteAmount;
	float3 BlindVisionG;
	float VignetteCurve;
	float3 BlindVisionB;
	float VignetteRadius;
	float3 VignetteColor;
	float LinearIntensity;
	float3 ColorGamma;
	float GammaIntensity;
	float3 DesaturationGamma;
	float DesaturationIntensity;
}

float3 GetToneMapping(float3 Pixel)
{
	float3 Color = Pixel / (Pixel + LinearIntensity) * GammaIntensity;
	return float3(dot(Color, BlindVisionR.xyz), dot(Color, BlindVisionG.xyz), dot(Color, BlindVisionB.xyz)) * ColorGamma;
}

float3 GetDesaturation(float3 Pixel)
{
	float3 Color = Pixel;
	float Intensity = DesaturationGamma.x * Color.r + DesaturationGamma.y * Color.b + DesaturationGamma.z * Color.g;
	Intensity *= DesaturationIntensity;

	Color.r = Intensity + Color.r * (1 - DesaturationIntensity);
	Color.g = Intensity + Color.g * (1 - DesaturationIntensity);
	Color.b = Intensity + Color.b * (1 - DesaturationIntensity);

	return Color;
}

float4 PS(VertexResult V) : SV_TARGET0
{
	float3 Color = GetDiffuse(V.TexCoord.xy).xyz;
	float Vignette = saturate(distance((V.TexCoord.xy + 0.5f) / VignetteRadius - 0.5f, -float2(0.5f, 0.5f)));
	Color = lerp(Color, VignetteColor.xyz, pow(Vignette, VignetteCurve) * VignetteAmount);
	Color = GetDesaturation(Color);

	return float4(GetToneMapping(Color), 1);
};