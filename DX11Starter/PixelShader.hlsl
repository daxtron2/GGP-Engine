// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage

struct Light
{
	float3 color;
	float intensity;
	float3 direction;
	int type; //0 - directional, 1-point
	float3 worldPos;
	float range;
};

cbuffer LightingData : register(b0)
{
	Light light1;
	Light light2;
	Light light3;
	float3 ambientColor;
	float specIntensity;
	float3 cameraPosition;
}
Texture2D diffuseTexture	:  register	(t0);// "t" registers
SamplerState samplerState	:  register	(s0);// "s" registers

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
	float3 worldPos		: WORLD_POSITION;
	float2 uv			: TEXCOORD;
};

float3 Diffuse(float3 normal, float3 dirToLight)
{
	return saturate(dot(normal, dirToLight));
}

float SpecularPhong(float3 normal, float3 dirOfLight, float3 toCam, float specularExponent)
{
	float3 reflection = reflect(dirOfLight, normal);

	float specularityAmt = saturate(dot(reflection, toCam));

	return pow(specularityAmt, specularExponent);
}

float3 CalculateLight(Light light, float3 normal, float3 color, float3 worldPos, float3 toCam)
{
	float3 dirToLight = float3(0, 0, 0);
	if (light.type == 0) //directional
	{
		dirToLight = normalize(-light.direction);
	}
	else if (light.type == 1) //point
	{
		dirToLight = normalize(light.worldPos - worldPos);
	}

	float3 diffuseAmt = Diffuse(normal, dirToLight);

	float specularity = SpecularPhong(normal, -dirToLight, toCam, 128.f);

	return diffuseAmt * light.color * color + specularity;
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
	
	float3 surfaceColor = diffuseTexture.Sample(samplerState, input.uv).rgb;
	
	float3 toCam = normalize(cameraPosition - input.worldPos);

	float3 light1Result = CalculateLight(light1, input.normal, surfaceColor, input.worldPos, toCam);
	float3 light2Result = CalculateLight(light2, input.normal, surfaceColor, input.worldPos, toCam);
	float3 light3Result = CalculateLight(light3, input.normal, surfaceColor, input.worldPos, toCam);

	float3 finalColor =
		surfaceColor * ambientColor.rgb +
		light1Result +
		light2Result +
		light3Result;

	return float4(finalColor, 1);
}