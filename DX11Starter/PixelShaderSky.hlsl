struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 sampleDir	: DIRECTION;
}; 

TextureCube textureCube		: register(t0);
SamplerState samplerOptions : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	return textureCube.Sample(samplerOptions, input.sampleDir);
}