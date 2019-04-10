struct VertexToPixel
{
	float4 svPosition : SV_POSITION;
};

Texture2D TexAlbedo : register(t0);
SamplerState Sampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float2 uv = input.svPosition.xy / 512.0;
    float4 albedo = TexAlbedo.Sample(Sampler, uv);
    return float4(albedo.rgb, 1.0f);
}