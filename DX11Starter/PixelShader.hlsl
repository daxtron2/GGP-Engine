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
Texture2D diffuseTexture	:  register	(t0);// "t" registers
SamplerState samplerState	:  register	(s0);// "s" registers

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
	return float4(CalculateFinalColor(input, samplerState, diffuseTexture, cameraPosition, light1, light2, light3, ambientColor), 1);
}