#pragma once
/* These buffers are shared across multiple shaders (Tessellation and Tessellation Depth). */

struct TessellationBufferType
{
	float nearDistance;
	float farDistance;
	float farLOD;
	float nearLOD;
	XMFLOAT3 cameraPosition;
	float padding;
};

struct HeightmapParamsBufferType
{
	float amplitude;
	XMFLOAT2 heightmapDimensions;
	float padding;
};