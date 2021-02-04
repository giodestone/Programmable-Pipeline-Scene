#pragma once
#include "DXF.h"

/// <summary>
/// Based off of the Plane Mesh, sends control patches, and contains extra control points with surrounding patches.
/// </summary>
class TessellationPlaneMesh : public BaseMesh
{
public:
	/// <summary>
	/// Create a control point mesh.
	/// </summary>
	/// <param name="device"></param>
	/// <param name="resolution">How big the plane will be.</param>
	TessellationPlaneMesh(ID3D11Device* device, int resolution=50);
	~TessellationPlaneMesh();

	/// <summary>
	/// Send the control point data to the device.
	/// </summary>
	/// <param name="deviceContext"></param>
	void sendData(ID3D11DeviceContext* deviceContext);

	int resolution;
protected:

	/// <summary>
	/// Setup the mesh vertices, normals and uv's.
	/// </summary>
	/// <param name="device"></param>
	void initBuffers(ID3D11Device* device) override;
};

