#pragma once
#include "BaseShader.h"

struct ShadowDepthShaderParams
{
	// Only to follow existing convention of having params!
};

/// <summary>
/// For outputting the depth of the point to a render texture.
/// </summary>
class ShadowDepthShader :
	public BaseShader
{
public:
	ShadowDepthShader(ID3D11Device* device, HWND hwnd);
	~ShadowDepthShader();

	/// <summary>
	/// Send data to the shaders.
	/// </summary>
	/// <param name="deviceContext"></param>
	/// <param name="worldMatrix">Mesh transform.</param>
	/// <param name="viewMatrix">Light view.</param>
	/// <param name="projectionMatrix">Ortho or projection depending on light.</param>
	/// <param name="shadowDepthShaderParams"></param>
	void setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, const ShadowDepthShaderParams& shadowDepthShaderParams);
protected:

	/// <summary>
	/// Compile shaders and setup buffers.
	/// </summary>
	/// <param name="vsPath"></param>
	/// <param name="psPath"></param>
	void initShader(const wchar_t* vsPath, const wchar_t* psPath) override;

	ID3D11Buffer* vsMatrixBuffer;
};

