struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
	float3 tangent : TANGENT;
};

struct VertexToPixel
{
    float4 Position : SV_POSITION;
};

cbuffer ShadowData : register(b0) {
	matrix world;
	matrix viewProjection;
}

VertexToPixel main(VertexShaderInput input)
{
    VertexToPixel output;

	output.Position = mul(float4(input.position, 1.0f), mul(world, viewProjection));

	return output;
}