#pragma once
#include "BaseShader.h"
#include "DXF.h"

struct BloomVeritcalBlurShaderParams
{
	ID3D11ShaderResourceView* textureToBlur;
	XMFLOAT2 textureDimensions;
	float intensityModifier;
};

/// <summary>
/// For vertically blurring and changing alpha.
/// </summary>
class BloomVerticalBlurShader :
	public BaseShader
{
	struct BlurWeightsBufferType
	{
		XMFLOAT2 textureDimensions;
		float intensityModifier;
		float padding;
	};

public:
	BloomVerticalBlurShader(ID3D11Device* device, HWND hwnd);
	~BloomVerticalBlurShader();

	/// <summary>
	/// Send relevant data to the shader.
	/// </summary>
	/// <param name="deviceContext"></param>
	/// <param name="worldMatrix"></param>
	/// <param name="viewMatrix"></param>
	/// <param name="orthoMatrix"></param>
	/// <param name="bloomVerticalBlurParams"></param>
	void setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & orthoMatrix, const BloomVeritcalBlurShaderParams & bloomVerticalBlurParams);
protected:
	/// <summary>
	/// Load shaders and setup buffers.
	/// </summary>
	/// <param name="vsPath"></param>
	/// <param name="psPath"></param>
	void initShader(const wchar_t* vsPath, const wchar_t* psPath) override;

	ID3D11Buffer* vsMatrixBuffer;
	ID3D11Buffer* psBlurWeightsBuffer;
	ID3D11SamplerState* psSamplerState;
};

