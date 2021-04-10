struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 sampleDir	: DIRECTION;
}; 

TextureCube cubeMap			: register(t0);
SamplerState samplerOptions : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	return cubeMap.Sample(samplerOptions, input.sampleDir);
}