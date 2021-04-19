#include "ShaderIncludes.hlsli"

cbuffer LightingData : register(b0)
{
	Light light1;
	Light light2;
	Light light3;
	float3 ambientColor;
	float specIntensity;
	float3 cameraPosition;
}

Texture2D albedoTexture		: register (t0);// "t" registers
Texture2D normalMap			: register (t1);
Texture2D roughnessMap		: register (t2);
Texture2D metalnessMap		: register (t3);
SamplerState samplerState	: register (s0);// "s" registers

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixelNormalMap input) : SV_TARGET
{
	float3 unpackedNormal = normalMap.Sample(samplerState, input.uv).rgb * 2 - 1;
	float3 norm = input.normal;

	float3 tan = input.tangent;
	tan = normalize(tan - norm * dot(tan, norm));

	float3 bitan = cross(tan, norm);
	float3x3 TBN = float3x3(tan, bitan, norm);

	input.normal = mul(unpackedNormal, TBN);

	VertexToPixel temp;
	temp.position = input.position;
	temp.color = input.color;
	temp.normal = input.normal;
	temp.worldPos = input.worldPos;
	temp.uv = input.uv;

	return float4(CalculateFinalColor(temp, samplerState, albedoTexture, roughnessMap, metalnessMap, cameraPosition, light1, light2, light3, ambientColor), 1);
}