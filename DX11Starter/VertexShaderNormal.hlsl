#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0) 
{ 
	float4 colorTint; 
	matrix world; 
	matrix view;
	matrix projection;
}



// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixelNormalMap main( VertexShaderInput input )
{
	return CalculateVertexToPixelNormalMap(input, colorTint, world, view, projection);
}