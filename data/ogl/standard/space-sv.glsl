#include "renderer/vertex/shape"
#include "workflow/pass"

VertexResult VS(VertexBase V)
{
	VertexResult Result = (VertexResult)0;
	Result.Position = V.Position;
	Result.TexCoord = Result.Position;

	return Result;
}