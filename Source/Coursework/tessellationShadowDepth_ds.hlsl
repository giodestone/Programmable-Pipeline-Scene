/* Tessellation Domain Shader */

Texture2D heightMapTex : register(t0);
SamplerState samplerState : register(s0);

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer HeightmapBuffer : register(b1)
{
    float amplitude;
    float2 heightmapSize;
    float padding;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
};

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    OutputType output;
	
    /*Find the position of the new vertex*/
	float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
	float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
    float3 vertexPosition = lerp(v1, v2, uvwCoord.x);

	/*Find the new postition of the texture with similar technqiue to above*/
    float2 uv1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.y);
    float2 uv2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.y);
    float2 uvPos = lerp(uv1, uv2, uvwCoord.x); // Required for sampling the heightmap.

    /*Calculate the position of the new vertex against the world, view, and projection matrices*/
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);

	/*Add the Y offset before view and projection matrices*/
    output.position.y = output.position.y + heightMapTex.SampleLevel(samplerState, uvPos, 0.0).r * amplitude;

    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	/*Now set the depth position.*/
	output.depthPosition = output.position;
	//REMEMBER NON UNIFORM SCALING REQUIRES SPECIAL MATRIX FOR NORMALS!!! (SEE NOTES)
    
	return output;
}