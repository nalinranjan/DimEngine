
struct DirectionalLight {
	float4 ambientColor;
	float4 diffuseColor;
	float3 direction;
};

struct PointLight {
	float4 ambientColor;
	float4 diffuseColor;
	float3 position;
};

cbuffer externalData : register(b0) {
	DirectionalLight directionalLight;
	DirectionalLight directionalLight2;
	PointLight pointLight;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Base color (texture sample)
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	// Directional Light
	float3 L = normalize(-directionalLight.direction);
	float3 N = normalize(input.normal);
	float NoL = saturate(dot(N, L));
	float3 lightColor = directionalLight.ambientColor.xyz + directionalLight.diffuseColor.xyz * NoL;

	// Directional Light 2
	L = normalize(-directionalLight2.direction);
	NoL = saturate(dot(N, L));
	lightColor += directionalLight2.ambientColor.xyz + directionalLight2.diffuseColor.xyz * NoL;

	// Point Light
	L = normalize(pointLight.position - input.worldPos);
	NoL = saturate(dot(N, L));
	lightColor += pointLight.ambientColor.xyz + pointLight.diffuseColor.xyz * NoL;

	return float4(surfaceColor * lightColor, 1);
}