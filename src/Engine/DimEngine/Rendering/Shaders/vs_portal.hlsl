cbuffer CameraData : register(b2)
{
	matrix view;
	matrix projection;
	matrix viewProjection;
};

cbuffer ObjectData : register(b3)
{
	matrix world;
};

struct VertexToPixel
{
	float4 svPosition : SV_POSITION;
};


float4 main(float3 position : POSITION) : SV_POSITION
{
	matrix WVP = mul(world, viewProjection);

	return mul(float4(position, 1.0f), WVP);
}