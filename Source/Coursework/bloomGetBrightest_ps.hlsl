Texture2D sceneTex : register(t0);
SamplerState sampler0 : register(s0);

cbuffer BloomBrightnessBuffer : register(b0)
{
    float intensityCutOff;
    float3 padding;
}

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	/*Sample texture*/
    float4 textureColour = sceneTex.Sample(sampler0, input.tex);

	/*Get overall intensity*/
    float overallIntensity = length(textureColour.xyz);

	/*Cutoff above a certain intensity*/
    if (overallIntensity < intensityCutOff)
        return float4(0.f, 0.f, 0.f, 1.f);

    return float4(textureColour.xyz, 0.f);
}