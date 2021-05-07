cbuffer ExternalData : register(b0)
{
	float2 pixelToUVSize;
}

struct VertexToPixel {
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

Texture2D inputLayer;
SamplerState samplerOptions;

float4 main(VertexToPixel input) : SV_TARGET
{
	// Define the number of pixels left/right to sample
	#define BLUR_RADIUS 4

	// Use a larger weight for pixels closer to the center
	const float weights[BLUR_RADIUS + 1] = {5, 4, 3, 2, 1};

	// Keep track of the total brightness vs. weight used
	float4 brightness = float4(0, 0, 0, 0);
	float totalWeight = 0;

	for (int x = -BLUR_RADIUS; x <= BLUR_RADIUS; x++) {
		for (int y = -BLUR_RADIUS; y <= BLUR_RADIUS; y++) {
			// Calculate how much of an impact this pixel's brightness will have
			// The higher a weight will go, the 
			float weight = weights[abs(x)] * weights[abs(y)];

			// Calculate the new UV position to sample including the X/Y offset
			float2 uvWithOffset = input.uv + float2(pixelToUVSize.x * x, pixelToUVSize.y * y);

			// Apply the brightness and increment the weight of the calculation
			brightness += inputLayer.Sample(samplerOptions, uvWithOffset) * weight;
			totalWeight += weight;
		}
	}

	// Divide the brightness by the weight of all pixels sampled
	brightness /= totalWeight;

	return brightness;
}