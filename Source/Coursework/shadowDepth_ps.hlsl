/* Just stores the depth value in a pixel map. */

struct InputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	/* Get the 0-1 depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate. */
	float depthValue = input.depthPosition.z / input.depthPosition.w;
	return float4(depthValue, depthValue, depthValue, 1.0f);
}