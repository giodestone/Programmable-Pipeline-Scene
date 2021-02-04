/*Adds lighting, and samples the texture.*/

Texture2D textureToMerge : register(t0);
Texture2D sceneTexture : register(t1);

SamplerState sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = saturate(textureToMerge.Sample(sampler0, input.tex) + sceneTexture.Sample(sampler0, input.tex));

	return textureColour;
}