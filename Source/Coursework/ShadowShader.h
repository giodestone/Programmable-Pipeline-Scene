#pragma once
#include "BaseShader.h"
#include "Light.h"
#include "ShadowShaderParams.h"
#include "ShadowShaderBufferTypes.h"

/// <summary>
/// For rendering a regular (non tessellated) object to scene with shadows.
/// </summary>
class ShadowShader :
	public BaseShader
{
	/*See shadow shader buffer types .h - shared with shadow tessellation shader!*/
public:
	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	/// <summary>
	/// Send parameters to shadow shader.
	/// </summary>
	/// <param name="deviceContext"></param>
	/// <param name="worldMatrix">Model transform.</param>
	/// <param name="viewMatrix">Camera view.</param>
	/// <param name="projMatrix">Camera projection.</param>
	/// <param name="shadowShaderParams"></param>
	void setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projMatrix, const ShadowShaderParams & shadowShaderParams);
protected:
	/// <summary>
	/// Load in shaders and setup buffers and samplers.
	/// </summary>
	/// <param name="vsPath"></param>
	/// <param name="psPath"></param>
	void initShader(const wchar_t* vsPath, const wchar_t* psPath) override;

	ID3D11Buffer* vsMatrixBuffer;
	ID3D11Buffer* vsLightMatrixBuffer;

	ID3D11Buffer* psLightAndShadowBuffer;
	ID3D11Buffer* psDirectionalLightBuffer;
	ID3D11Buffer* psPointLightBuffer;
	ID3D11Buffer* psSpotLightBuffer;
	ID3D11SamplerState* psSamplerState;
};

