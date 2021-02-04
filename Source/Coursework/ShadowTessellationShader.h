#pragma once
#include "BaseShader.h"
#include "PlaneTessellationParams.h"
#include "TessellationBufferTypes.h"
#include "ShadowShaderParams.h"
#include "ShadowShaderBufferTypes.h"

/// <summary>
/// For drawing the tessellated plane with shadows.
/// </summary>
class ShadowTessellationShader : public BaseShader
{
	/*Shadow shader buffer types contains the buffers that would usually be here - shared with shadow shader.*/
public:
	ShadowTessellationShader(ID3D11Device* device, HWND hwnd);
	~ShadowTessellationShader();


	/// <summary>
	/// Send relevant data to the shaders.
	/// </summary>
	/// <param name="deviceContext"></param>
	/// <param name="worldMatrix">Model matrix.</param>
	/// <param name="viewMatrix">Camera view.</param>
	/// <param name="projectionMatrix">Camera projection.</param>
	/// <param name="planeTessellationParams"></param>
	/// <param name="shadowShaderParams"></param>
	void setShaderParams(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, const PlaneTessellationParams& planeTessellationParams, const ShadowShaderParams& shadowShaderParams);

protected:
	/// <summary>
	/// Only for satisfying the abstract override. Use other initShader.
	/// </summary>
	/// <param name=""></param>
	/// <param name=""></param>
	void initShader(const wchar_t *, const wchar_t *) override;

	/// <summary>
	/// Loads shaders and sets up buffers.
	/// </summary>
	/// <param name="vsPath"></param>
	/// <param name="hsPath"></param>
	/// <param name="dsPath"></param>
	/// <param name="psPath"></param>
	void initShader(const wchar_t* vsPath, const wchar_t* hsPath, const wchar_t* dsPath, const wchar_t* psPath);

	ID3D11Buffer* hsTessellationBuffer;

	ID3D11Buffer* dsMatrixBuffer;
	ID3D11Buffer* dsHeightmapParamsBuffer;
	ID3D11Buffer* dsLightMatrixBuffer;
	ID3D11SamplerState* dsHeightMapSamplerState;

	ID3D11Buffer* psLightAndShadowBuffer;
	ID3D11SamplerState* psSamplerState;
	ID3D11Buffer* psDirectionalLightBuffer;
	ID3D11Buffer* psPointLightBuffer;
	ID3D11Buffer* psSpotLightBuffer;
};

