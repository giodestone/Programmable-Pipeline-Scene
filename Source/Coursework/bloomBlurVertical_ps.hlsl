Texture2D sceneTex : register(t0);
SamplerState sampler0 : register(s0);

cbuffer BlurWeightsBuffer : register(b0)
{
    float2 sceneTexSize;
	float intensityModifier;
    float padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
    float2 onePx = float2(1.f / sceneTexSize.x, 1.f / sceneTexSize.y);

    const int noOfWeights = 12;
    
    float weights[noOfWeights] = //24px blur
    {
        0.2849588920625741,
		0.2713338582778863,
		0.23424445595067667,
		0.1833488562175991,
		0.13011598174197048,
		0.08371950162826163,
		0.04883893438631658,
		0.025831479334607353,
		0.012387279327401902,
		0.005385749341235311,
		0.0021230484085511866,
		0.0007587823326499773,
    };
	
    float4 color = float4(0.f, 0.f, 0.f, 0.f);
	/* Sample around texture using the weights .*/
    for (int i = -(noOfWeights - 1); i < noOfWeights; ++i)
    {
        color += sceneTex.Sample(sampler0, float2(input.tex.x, input.tex.y + (onePx.y * ((float) i)))) * weights[abs(i)];
    }
	color.w = saturate(length(color.xyz) / 3.f); // Scale the alpha based on the color strength (more needs to be blended in the more color there is).

    color.xyz = saturate(color.xyz / intensityModifier);

    return color;
}

/* Code for generating weights, self made ruby script. Change stdev to get more/less values. Written by me.
	
	https://gist.github.com/giodestone/f1e04262bf467436ff745fd180ec12d1
*/