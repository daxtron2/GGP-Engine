#ifndef __GGP_SHADER_INCLUDES__
#define __GGP_SHADER_INCLUDES__

#include "PBRIncludes.hlsli"



// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

// Struct representing the data we're sending down the pipeline
// - Should match our pixel shader's input (hence the name: Vertex to Pixel)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float3 normal		: NORMAL;
	float3 worldPos		: WORLD_POSITION;
	float2 uv			: TEXCOORD;
};

struct VertexToPixelNormalMap
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float3 normal		: NORMAL;
	float3 worldPos		: WORLD_POSITION;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

VertexToPixel CalculateVertexToPixel(VertexShaderInput input, float4 colorTint, matrix world, matrix view, matrix projection)
{
	// Set up output struct
	VertexToPixel output;

	// Here we're essentially passing the input position directly through to the next
	// stage (rasterizer), though it needs to be a 4-component vector now.  
	// - To be considered within the bounds of the screen, the X and Y components 
	//   must be between -1 and 1.  
	// - The Z component must be between 0 and 1.  
	// - Each of these components is then automatically divided by the W component, 
	//   which we're leaving at 1.0 for now (this is more useful when dealing with 
	//   a perspective projection matrix, which we'll get to in future assignments).
	matrix wvp = mul(projection, mul(view, world));
	output.position = mul(wvp, float4(input.position, 1.0f));
	output.color = colorTint;
	output.normal = normalize(mul((float3x3)world, input.normal));
	output.worldPos = mul(world, float4(input.position, 1.0f)).xyz;
	output.uv = input.uv;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}

VertexToPixelNormalMap CalculateVertexToPixelNormalMap(VertexShaderInput input, float4 colorTint, matrix world, matrix view, matrix projection)
{
	VertexToPixel temp = CalculateVertexToPixel(input, colorTint, world, view, projection);
	
	VertexToPixelNormalMap output;
	output.position = temp.position;
	output.color = temp.color;
	output.normal = temp.normal;
	output.worldPos = temp.worldPos;
	output.uv = temp.uv;

	output.tangent = normalize(mul((float3x3)world, input.tangent));


	return output;
}

struct Light
{
	float3 color;
	float intensity;
	float3 direction;
	int type; //0 - directional, 1-point, 2-spot
	float3 worldPos;
	float range;
};

float Diffuse(float3 normal, float3 dirToLight)
{
	return saturate(dot(normal, dirToLight));
}

float SpecularPhong(float3 normal, float3 dirOfLight, float3 toCam, float specularExponent)
{
	float3 reflection = reflect(dirOfLight, normal);

	float specularityAmt = saturate(dot(reflection, toCam));

	return pow(specularityAmt, specularExponent);
}

float3 CalculateLight(Light light, float3 normal, float3 surfaceColor, float3 worldPos, float3 toCam, float roughness, float metalness, float3 specColor)
{
	float3 dirToLight = float3(0, 0, 0);
	if (light.type == 0) //directional
	{
		dirToLight = normalize(-light.direction);
	}
	else if (light.type == 1 || light.type == 2) //point or spot
	{
		dirToLight = normalize(light.worldPos - worldPos);
	}
	float spotAmount = 1.f;
	float coneAttenuation = 1.f;
	if (light.type == 2) {
		float angleFromCenter = max(dot(dirToLight, light.direction), 0.0f);

		spotAmount = pow(angleFromCenter, light.intensity);
		coneAttenuation = saturate((angleFromCenter - 3.f) / 10.f);
		coneAttenuation *= coneAttenuation;
	}

	float diffuseAmt = Diffuse(normal, dirToLight);

	//old phong specularity
	//float specularity = SpecularPhong(normal, -dirToLight, toCam, 128.f);

	//new pbr specularity
	float3 specularity = MicrofacetBRDF(normal, dirToLight, toCam, roughness, metalness, specColor);

	specularity *= any(diffuseAmt);

	float3 balancedDiff = DiffuseEnergyConserve(diffuseAmt, specularity, metalness);

	return (balancedDiff * surfaceColor + specularity) * light.intensity * light.color * spotAmount;
}


float3 CalculateFinalColor(VertexToPixel input, SamplerState samplerState, Texture2D albedoTexture, Texture2D roughnessMap, Texture2D metalnessMap,
						   float3 cameraPosition, Light light1, Light light2, Light light3, float3 ambientColor)
{
	input.normal = normalize(input.normal);

	float3 surfaceColor = pow(albedoTexture.Sample(samplerState, input.uv).rgb, 2.2f);
	float roughness = roughnessMap.Sample(samplerState, input.uv).r;
	float metalness = metalnessMap.Sample(samplerState, input.uv).r;
	float3 specularColor = lerp(F0_NON_METAL.rrr, surfaceColor.rgb, metalness);


	float3 toCam = normalize(cameraPosition - input.worldPos);

	float3 finalColor = surfaceColor * ambientColor.rgb +
			CalculateLight(light1, input.normal, surfaceColor, input.worldPos, toCam, roughness, metalness, specularColor) +
			CalculateLight(light2, input.normal, surfaceColor, input.worldPos, toCam, roughness, metalness, specularColor) +
			CalculateLight(light3, input.normal, surfaceColor, input.worldPos, toCam, roughness, metalness, specularColor);

	//apply gamma correction
	return pow(finalColor, 1.0f / 2.2f);
}


#endif