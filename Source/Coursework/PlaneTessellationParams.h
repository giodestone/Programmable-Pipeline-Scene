#pragma once
#include "DXF.h"

/// <summary>
/// Paramaters for the dynamic tessellation of the plane.
/// </summary>
struct PlaneTessellationParams
{
	XMFLOAT3 cameraPosition;
	ID3D11ShaderResourceView* heightmap;
	ID3D11ShaderResourceView* texture;

	float nearDistance; // At what distance will nearLOD be.
	float farDistance; // At what distance will farLOD be.
	float farLOD; // Tessellation at far distance and beyond.
	float nearLOD; // Tessellation at near distance and closer.
	
	float amplitude;
	XMFLOAT2 heightmapDimensions;
};