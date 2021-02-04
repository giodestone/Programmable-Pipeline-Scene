/* For tessellating the plane */

cbuffer TessellationBuffer : register(b0)
{
	float nearDistance;
	float farDistance;
	float farLOD;
	float nearLOD;
	float3 cameraPos;
	float padding;
};

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

/* Get average for four points. */
float3 getAverage(float3 p0, float3 p1, float3 p2, float3 p3)
{
    return (p0 + p1 + p2 + p3) / 4.f;
}

/* Get a the tessellation factor for the point based on the camera position. */
float getLODForPoint(float3 p, float3 cameraPos)
{
    float distance = length(cameraPos - p);
	
	//[Enhancement] TODO: MOVE ALONG EXPONENTIAL/TAN LINE TO MIMIC DISTANCE IN PERSPECTIVE
	float percentBetweenMinMax = 1.f - ((distance - nearDistance) / (farDistance - nearDistance));
	
	return clamp(lerp(farLOD, nearLOD, percentBetweenMinMax), farLOD, nearLOD); // Make sure that the tessellation factor doesn't go out of bounds. FarLOD is smaller than nearLOD.
}

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 12> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputType output;

	/* Calculate the mid points of this patch and surrounding ones. */
    float3 midPoints[] = {
        getAverage(inputPatch[0].position, inputPatch[1].position, inputPatch[2].position, inputPatch[3].position), //Main patch
		getAverage(inputPatch[2].position, inputPatch[3].position, inputPatch[4].position, inputPatch[5].position), //+x
		getAverage(inputPatch[1].position, inputPatch[3].position, inputPatch[6].position, inputPatch[7].position), //+z
		getAverage(inputPatch[0].position, inputPatch[1].position, inputPatch[8].position, inputPatch[9].position), //-x
		getAverage(inputPatch[0].position, inputPatch[2].position, inputPatch[10].position, inputPatch[11].position) //-z
    };

	/* Determine LOD for patch and surrounding ones. */
    float patchLODs[]= {
        getLODForPoint(midPoints[0], cameraPos), //Main patch
        getLODForPoint(midPoints[1], cameraPos), //+x
        getLODForPoint(midPoints[2], cameraPos), //+z
        getLODForPoint(midPoints[3], cameraPos), //-x
        getLODForPoint(midPoints[4], cameraPos), //-z
    };

    // Set the tessellation factors for the four edges of the quad to match the the LOD's for the four adjacent ones.
    output.edges[0] = min(patchLODs[0], patchLODs[4]); //top edge
    output.edges[1] = min(patchLODs[0], patchLODs[3]); //left edge
    output.edges[2] = min(patchLODs[0], patchLODs[2]); //bottom edge
    output.edges[3] = min(patchLODs[0], patchLODs[1]); //right edge

    // Set the tessellation factor for tessallating inside the quad.
    output.inside[0] = patchLODs[0]; //0 is the mid point
    output.inside[1] = patchLODs[0];

    return output;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 12> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;
    output.normal = patch[pointId].normal;
    output.tex = patch[pointId].tex;

    return output;
}