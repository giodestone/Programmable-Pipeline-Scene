#pragma once
#include "BaseShader.h"

struct BloomMergeToSceneShaderParams
{
	ID3D11ShaderResourceView* textureToMerge;
	ID3D11ShaderResourceView* sceneTexture;
	XMFLOAT2 textureToMergeDimensions;
};

class BloomMergeToSceneShader :
	public BaseShader
{
	struct BloomMergeBufferType
	{
		XMFLOAT2 sceneTextureDimensions;
		XMFLOAT2 textureToMergeDimensions;
	};

public:
	BloomMergeToSceneShader(ID3D11Device* device, HWND hwnd);
	~BloomMergeToSceneShader();

	void setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & orthoMatrix, const BloomMergeToSceneShaderParams & bloomMergeToSceneShaderParams);
protected:
	void initShader(const wchar_t* vsPath, const wchar_t* psPath) override;

	ID3D11Buffer* vsMatrixBuffer;
	ID3D11Buffer* psBlurWeightsBuffer;
	ID3D11SamplerState* psSamplerState;
};

