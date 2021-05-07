cbuffer externalData : register(b0)
{
	float bloomThreshold;
}

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

Texture2D drawTexture		: register(t0);
SamplerState samplerOptions : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
	// Given an input texture, if all RGB values are under a given threshold, return (0, 0, 0)
	// float4 brightness = drawTexture.Sample(samplerOptions, input.uv);

	//if (brightness.x >= bloomThreshold || brightness.y >= bloomThreshold || brightness.z >= bloomThreshold)
	//	return float4(brightness.x, brightness.x, brightness.x, 1.0f);

	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}