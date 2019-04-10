struct VertexToPixel
{
	float4 svPosition : SV_POSITION;
};

struct LightSource
{
	float4 ambientColor;
	float4 diffuseColor;
	int type;
	float3 position;
	float range;
	float3 direction;
};

cbuffer LightSourceData : register(b1)
{
	LightSource light;
};

cbuffer CameraData : register(b2)
{
    float3 cameraPosition;
};

Texture2D TexAlbedo : register(t0);
SamplerState Sampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float2 uv = input.svPosition.xy / input.svPosition.w;
    float4 albedo = TexAlbedo.Sample(Sampler, uv);
    return float4(albedo.rgb, 1.0f);
}