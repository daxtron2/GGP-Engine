cbuffer externalData : register(b0)
{
	float intensityBloomLayer0;
	float intensityBloomLayer1;
	float intensityBloomLayer2;
}

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
}; 

Texture2D preBloomPixels;
Texture2D bloomLayer0;
Texture2D bloomLayer1;
Texture2D bloomLayer2;
SamplerState samplerOptions;

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 brightness = preBloomPixels.Sample(samplerOptions, input.uv);

	brightness += bloomLayer0.Sample(samplerOptions, input.uv) * intensityBloomLayer0;
	brightness += bloomLayer1.Sample(samplerOptions, input.uv) * intensityBloomLayer1;
	brightness += bloomLayer2.Sample(samplerOptions, input.uv) * intensityBloomLayer2;

	return brightness ;
}