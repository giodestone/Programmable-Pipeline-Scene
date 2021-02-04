#define NO_OF_SHADOWED_LIGHTS 2

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

cbuffer LightMatrixBuffer : register(b1)
{
	matrix lightViewMatrix[NO_OF_SHADOWED_LIGHTS];
	matrix lightProjectionMatrix[NO_OF_SHADOWED_LIGHTS];
}

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPos[NO_OF_SHADOWED_LIGHTS] : TEXCOORD1;
	float4 worldPosition : TEXCOORD10;
};

OutputType main(InputType input)
{
	OutputType output;
	
	/* Calculate position of the point versus the matrices. */ 
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	for (int i = 0; i < NO_OF_SHADOWED_LIGHTS; i++)
	{
		/* Calculate the position of the point from the light point. */
		output.lightViewPos[i] = mul(input.position, worldMatrix);
		output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
		output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
	}
	
	//[Enhancement] TODO: If the scaling matrix is not equal in all axes need to do inverse of the world matrix to correclty calculate this.
	
	/* Pass on other data. */
	output.tex = input.tex;
	output.normal = mul(input.normal, (float3x3) worldMatrix); 
	output.normal = normalize(output.normal);
	
	output.worldPosition = mul(input.position, worldMatrix);

	return output;
}