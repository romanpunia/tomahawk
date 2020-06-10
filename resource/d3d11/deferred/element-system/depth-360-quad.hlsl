#include "renderer/element"
#include "renderer/vertex/element"
#include "renderer/buffer/object"
#include "renderer/input/element"
#include "geometry/forward"

cbuffer RenderConstant : register(b3)
{
	matrix FaceView[6];
};

VertexResult360 Make(in VertexResult90 V, in float2 Offset, in float2 TexCoord2, in uint i)
{
	float Sin = sin(V.Rotation), Cos = cos(V.Rotation);
	V.Position = mul(V.Position, FaceView[i]);
	V.Position += float4(Offset.x * Cos - Offset.y * Sin, Offset.x * Sin + Offset.y * Cos, 0, 0);
	V.Position = mul(V.Position, Projection);

	VertexResult360 Result = (VertexResult360)0;
	Result.Position = V.Position;
	Result.UV = V.UV;
	Result.Rotation = V.Rotation;
	Result.Scale = V.Scale;
	Result.Alpha = V.Alpha;
	Result.RenderTarget = i;
	Result.TexCoord = TexCoord2;

	return Result;
}

[maxvertexcount(24)]
void GS(point VertexResult90 V[1], inout TriangleStream<VertexResult360> Stream)
{
	for (uint i = 0; i < 6; i++)
	{
		Stream.Append(Make(V[0], float2(1, -1) * V[0].Scale, float2(0, 0), i));
		Stream.Append(Make(V[0], float2(1, 1) * V[0].Scale, float2(0, -1), i));
		Stream.Append(Make(V[0], float2(-1, -1) * V[0].Scale, float2(1, 0), i));
		Stream.Append(Make(V[0], float2(-1, 1) * V[0].Scale, float2(1, -1), i));
		Stream.RestartStrip();
	}
}

VertexResult90 VS(VertexBase V)
{
	VertexResult90 Result = (VertexResult90)0;
	Result.Position = Result.UV = mul(float4(Elements[V.Position].Position, 1), World);
	Result.Rotation = Elements[V.Position].Rotation;
	Result.Scale = Elements[V.Position].Scale;
	Result.Alpha = Elements[V.Position].Color.w;
    
	return Result;
}

float4 PS(VertexResult90 V) : SV_TARGET0
{
    Material Mat = GetMaterial(MaterialId);

	float Alpha = Mat.Transparency.x * V.Alpha;
	[branch] if (HasDiffuse > 0)
		Alpha *= GetDiffuse(V.TexCoord).w;

	[branch] if (Alpha < Mat.Transparency.y)
		discard;

	return float4(length(V.UV.xyz - ViewPosition) / FarPlane, Alpha, 1.0f, 1.0f);
};