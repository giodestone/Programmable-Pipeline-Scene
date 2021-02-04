#pragma once
#include "DXF.h"

struct PlaneTessellationShaderParams
{
	XMFLOAT3 cameraPosition;
	ID3D11ShaderResourceView* heightmap;
	ID3D11ShaderResourceView* texture;

	float minDistance;
	float maxDistance;
	float minLOD;
	float maxLOD;

	float amplitude;
	XMFLOAT2 heightmapDimensions;
};

/// <summary>
/// The plane tessellation shader WITHOUT shadows (mostly used for debugging earlier).
/// </summary>
class PlaneTessellationShader :
	public BaseShader
{
	struct TessellationBufferType
	{
		float minDistance;
		float maxDistance;
		float minLOD;
		float maxLOD;
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct HeightmapParamsBufferType
	{
		float amplitude;
		XMFLOAT2 heightmapDimensions;
		float padding;
	};

public:
	PlaneTessellationShader(ID3D11Device* device, HWND hwnd);
	~PlaneTessellationShader();

protected:
	/// <summary>
	/// Only for satisfying the abstract.
	/// </summary>
	void initShader(const wchar_t*, const wchar_t*) override;

	/// <summary>
	/// Initialise the shader by loading the shader stages in.
	/// </summary>
	/// <param name="vs">Path to vertex shader.</param>
	/// <param name="hs">Path to hull shader.</param>
	/// <param name="ds">Path to domain shader.</param>
	/// <param name="ps">Path to pixel shader.</param>
	void initShader(const wchar_t* vs, const wchar_t* hs, const wchar_t* ds, const wchar_t* ps);

public:
	/// <summary>
	/// Set the paramaters for the tessellation shader. Takes a ccw quad control point list.
	/// </summary>
	void setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, const PlaneTessellationShaderParams & planeShaderParams);

private:
	ID3D11Buffer* hsTessellationBuffer;
	
	ID3D11Buffer* dsMatrixBuffer;
	ID3D11Buffer* dsHeightmapParamsBuffer;
	ID3D11SamplerState* dsHeightMapSamplerState;

	ID3D11SamplerState* psTextureSamplerState;
};

