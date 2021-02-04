#include "BloomHorizontalBlurShader.h"
#include "ShaderHelpers.h"

BloomHorizontalBlurShader::BloomHorizontalBlurShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"general_vs.cso", L"bloomBlurHorizontal_ps.cso");
}


BloomHorizontalBlurShader::~BloomHorizontalBlurShader()
{
}

void BloomHorizontalBlurShader::setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & orthoMatrix, const BloomHorizontalBlurShaderParams & bloomHorizontalBlurParams)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	XMMATRIX tworld = XMMatrixTranspose(worldMatrix), tview = XMMatrixTranspose(viewMatrix), tortho = XMMatrixTranspose(orthoMatrix);

	/*Send data to VS.*/
	{
		//Matrix buffer
		result = deviceContext->Map(vsMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		MatrixBufferType* matrixBufferDataPtr = (MatrixBufferType*)mappedResource.pData;

		matrixBufferDataPtr->world = tworld;
		matrixBufferDataPtr->view = tview;
		matrixBufferDataPtr->projection = tortho;

		deviceContext->Unmap(vsMatrixBuffer, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &vsMatrixBuffer);
	}

	/*Send data to PS.*/
	{
		//Blur Weights Buffer
		result = deviceContext->Map(psBlurWeightsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		BlurWeightsBufferType* blurWeightsBufferDataPtr = (BlurWeightsBufferType*)mappedResource.pData;

		blurWeightsBufferDataPtr->textureDimensions = bloomHorizontalBlurParams.textureDimensions;
		blurWeightsBufferDataPtr->intensityModifier = bloomHorizontalBlurParams.intensityModifier;

		deviceContext->Unmap(psBlurWeightsBuffer, 0);
		deviceContext->PSSetConstantBuffers(0, 1, &psBlurWeightsBuffer);

		//TextureToBlur
		deviceContext->PSSetShaderResources(0, 1, &bloomHorizontalBlurParams.textureToBlurHorizontally);

		//Sampler
		deviceContext->PSSetSamplers(0, 1, &psSamplerState);
	}

}

void BloomHorizontalBlurShader::initShader(const wchar_t * vsPath, const wchar_t * psPath)
{
	/*Load VS and PS.*/
	loadVertexShader(vsPath);
	loadPixelShader(psPath);

	/*Setup VS.*/
	ShaderHelpers::setUpConstantBuffer<MatrixBufferType>(renderer, &vsMatrixBuffer);

	/*Setup PS.*/
	ShaderHelpers::setUpConstantBuffer<BlurWeightsBufferType>(renderer, &psBlurWeightsBuffer);
	ShaderHelpers::setUpSampleState(renderer, &psSamplerState);
}
