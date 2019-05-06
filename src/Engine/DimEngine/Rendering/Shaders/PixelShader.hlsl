struct VertexToPixel
{
    float4 svPosition : SV_POSITION;
    float4 position : POSITION;
    float3 normal : NORMAL;
	float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
	float4 shadowPos : SHADOW;
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

cbuffer EnableMaps : register(b0) {
	int hasTexture;
	int hasNormalMap;
	int hasShadowMap;
	int hasMetalnessMap;
	int hasRoughnessMap;
}

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
Texture2D normalMap : register(t1);
Texture2D ShadowMap	: register(t2);

SamplerComparisonState shadowSampler : register(s1);

float4 Lambert(float4 ambientColor, float4 diffuseColor, float3 N, float3 L)
{
    return ambientColor + saturate(dot(N, L)) * diffuseColor;
}

float4 BlinnPhong(float3 N, float3 L, float3 V, float shininess)
{
    return pow(saturate(dot(-normalize(reflect(L, N)), V)), shininess).xxxx;
}


float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	//normal map
	//change the normal of each point
	float3 textureNormal = normalMap.Sample(Sampler, input.uv).rgb * 2 - 1;

	float3 N = input.normal;
	float3 T = input.tangent;
	float3 B = cross(T, N);

	float3x3 TBN = float3x3(T, B, N);

	float3 normal = normalize(mul(textureNormal, TBN));
	input.normal = lerp(input.normal, normal, hasNormalMap);

    float4 albedo = TexAlbedo.Sample(Sampler, input.uv);

    N = normalize(input.normal);
    float3 L = -normalize(light.direction);
    float3 V = normalize(cameraPosition.xyz - input.position.xyz);

	float2 shadowUV = input.shadowPos.xy / input.shadowPos.w * 0.5f + 0.5f;
	shadowUV.y = 1.0f - shadowUV.y;
	float depthFromLight = input.shadowPos.z / input.shadowPos.w;
	float shadowAmount = ShadowMap.SampleCmpLevelZero(shadowSampler, shadowUV, depthFromLight);

	return float4(shadowAmount.rrr, 1.0f);

	//return float4(hasNormalMap.rrr, 1.0f);
    return float4(((albedo * Lambert(light.ambientColor, light.diffuseColor, N, L) + BlinnPhong(N, L, V, 16)).rgb) *shadowAmount, 1.0f);
}