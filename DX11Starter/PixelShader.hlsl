// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage

struct DirectionalLight 
{
	float3 Color;
	float Intensity;
	float3 Direction;
};

cbuffer LightingData : register(b0)
{
	DirectionalLight light1;
	DirectionalLight light2;
	DirectionalLight light3;
	float3 ambientColor;
}

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
};

float3 Diffuse(float3 normal, float3 dirToLight)
{
	return saturate(dot(normal, dirToLight));
}

float3 CalculateLight(DirectionalLight light, float3 normal, float4 surfaceColor)
{
	float3 dirToLight = normalize(-light.Direction);
	float3 diffuseAmt = Diffuse(normal, dirToLight);

	return diffuseAmt * light.Color * (float3)surfaceColor;
}

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
	input.normal = normalize(input.normal);
	
	float3 light1Result = CalculateLight(light1, input.normal, input.color);
	float3 light2Result = CalculateLight(light2, input.normal, input.color);
	float3 light3Result = CalculateLight(light3, input.normal, input.color);
	float3 finalLightResult = light1Result + light2Result + light3Result;
	float3 ambientLightResult = (float3)ambientColor * (float3)input.color;

	//return float4(light3Result, 1);

	return float4(finalLightResult + ambientLightResult, 1);
}

