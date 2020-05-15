struct VertexOut
{
    float4 Position : SV_POSITION0;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BINORMAL0;
    float4 RawPosition : TEXCOORD1;
};