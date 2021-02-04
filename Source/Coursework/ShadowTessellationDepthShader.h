#pragma once
#include "BaseShader.h"
#include "PlaneTessellationParams.h"
#include "TessellationBufferTypes.h"

/// <summary>
/// For rendering the depth of the tessellated terrian to the depth shader.
/// </summary>
class ShadowTessellationDepthShader : public BaseShader
{
public:
	ShadowTessellationDepthShader(ID3D11Device* device, HWND hwnd);
	~ShadowTessellationDepthShader();

	/// <summary>
	/// Send data to shaders.
	/// </summary>
	/// <param name="deviceContext"></param>
	/// <param name="worldMatrix">Model transform.</param>
	/// <param name="viewMatrix">Light view matrix.</param>
	/// <param name="projectionMatrix">Light ortho/projection.</param>
	/// <param name="planeTessellationParams"></param>
	void setShaderParams(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, const PlaneTessellationParams& planeTessellationParams);
protected:
	/// <summary>
	/// Only to satisfy the abstract requirement (call the other init shader).
	/// </summary>
	/// <param name=""></param>
	/// <param name=""></param>
	void initShader(const wchar_t *, const wchar_t *) override;

	/// <summary>
	/// Load shaders and setup buffers.
	/// </summary>
	/// <param name="vsPath"></param>
	/// <param name="hsPath"></param>
	/// <param name="dsPath"></param>
	/// <param name="psPath"></param>
	void initShader(const wchar_t* vsPath, const wchar_t* hsPath, const wchar_t* dsPath, const wchar_t* psPath);

	ID3D11Buffer* hsTessellationBuffer;

	ID3D11Buffer* dsMatrixBuffer;
	ID3D11Buffer* dsHeightmapParamsBuffer;
	ID3D11SamplerState* dsHeightMapSamplerState;
};

