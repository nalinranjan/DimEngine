struct VertexToPixel {
	float4 svPosition : SV_POSITION;
};

cbuffer GlobalData : register(b0)
{
	float2 screenSize;
}

Texture2D TexAlbedo : register(t0);

SamplerState Sampler : register(s0);

float4 main(float4 svPosition : SV_POSITION) : SV_TARGET
{
	float2 uv = svPosition / float2(1280, 720);
    float4 albedo = TexAlbedo.Sample(Sampler, uv);
    
	return float4(albedo.rgb, 1.0f);
}