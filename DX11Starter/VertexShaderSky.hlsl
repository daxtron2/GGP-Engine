struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 sampleDir	: DIRECTION;
};

cbuffer ViewProjData : register(b0)
{
	matrix view;
	matrix projection;
}

VertexToPixel main(VertexShaderInput input )
{
	VertexToPixel output;

	matrix viewCopy = view;
	viewCopy._14 = 0;
	viewCopy._24 = 0;
	viewCopy._34 = 0;

	matrix vp = mul(projection, viewCopy);

	output.position = mul(vp, float4(input.position, 1));
	
	output.position.z = output.position.w;

	output.sampleDir = input.position;

	return output;
}