#pragma once
#include "BaseShader.h"
#include "DXF.h"

struct BloomHorizontalBlurShaderParams
{
	ID3D11ShaderResourceView* textureToBlurHorizontally;
	XMFLOAT2 textureDimensions;
	float intensityModifier;
};

/// <summary>
/// For horizontally blurring and changing alpha.
/// </summary>
class BloomHorizontalBlurShader :
	public BaseShader
{
	struct BlurWeightsBufferType
	{
		XMFLOAT2 textureDimensions;
		float intensityModifier;
		float padding;
	};

public:
	BloomHorizontalBlurShader(ID3D11Device* device, HWND hwnd);
	~BloomHorizontalBlurShader();

	/// <summary>
	/// Send relevant data to shader.
	/// </summary>
	/// <param name="deviceContext"></param>
	/// <param name="worldMatrix"></param>
	/// <param name="viewMatrix"></param>
	/// <param name="orthoMatrix"></param>
	/// <param name="bloomHorizontalBlurParams"></param>
	void setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & orthoMatrix, const BloomHorizontalBlurShaderParams & bloomHorizontalBlurParams);
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

