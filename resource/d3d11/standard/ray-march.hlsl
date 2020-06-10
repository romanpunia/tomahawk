#include "geometry/sv"

float RayEdgeSample(in float2 TexCoord)
{
    float2 Coord = smoothstep(0.2, 0.6, abs(float2(0.5, 0.5) - TexCoord));
    return clamp(1.0 - (Coord.x + Coord.y), 0.0, 1.0);
}
bool RayMarch(inout float3 Position, inout float3 Direction, in float IterationCount, out float2 HitCoord)
{
    float4 Coord; float Depth;
    const float Bias = 0.000001;
    Position += Direction;

	[loop] for (int i = 0; i < IterationCount; i++)
	{
		Coord = mul(float4(Position, 1.0), ViewProjection);
		Coord.xy = float2(0.5, 0.5) + float2(0.5f, -0.5) * Coord.xy / Coord.w;
		Coord.z /= Coord.w;
		
		Depth = Coord.z - GetDepth(Coord.xy);
		[branch] if (abs(Depth) < Bias)
		{
			HitCoord = Coord.xy;
            return true;
		}

        [branch] if (Depth > 0)
            Direction *= 0.5;

		Position -= Direction * sign(Depth);
	}
	
	return false;
}