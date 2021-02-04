/*Adds lighting, and samples the texture.*/

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = texture0.Sample(sampler0, input.tex);
	lightColour = calculateLighting(-(float3(0.77f, -0.77f, 0.f)), input.normal, float4(0.8f, 0.8f, 0.8f, 0.f));

	return lightColour * textureColour;
}