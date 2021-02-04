#pragma once
#include "DXF.h"

struct BloomGetBrightestShaderParams
{
	float intensityCutOff;
	ID3D11ShaderResourceView* sceneTexture;
};

/// <summary>
/// For getting the brightest parts of the scene and rendering to texture.
/// </summary>
class BloomGetBrightestShader :
	public BaseShader
{
	struct BloomBrightnessBufferType
	{
		float intensityCutOff;
		XMFLOAT3 padding;
	};

public:
	BloomGetBrightestShader(ID3D11Device* device, HWND hwnd);
	~BloomGetBrightestShader();

	/// <summary>
	/// Set paramaters and set data to shader.
	/// </summary>
	/// <param name="deviceContext"></param>
	/// <param name="worldMatrix"></param>
	/// <param name="viewMatrix"></param>
	/// <param name="orthoMatrix"></param>
	/// <param name="bloomBrightnessParams"></param>
	void setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & orthoMatrix, const BloomGetBrightestShaderParams & bloomBrightnessParams);

protected:
	/// <summary>
	/// Initialise buffers and load shaders.
	/// </summary>
	/// <param name="vsPath"></param>
	/// <param name="psPath"></param>
	void initShader(const wchar_t* vsPath, const wchar_t* psPath) override;

	ID3D11Buffer* vsMatrixBuffer;
	ID3D11Buffer* psBrightnessBuffer;
	ID3D11SamplerState* psSamplerState;
};

