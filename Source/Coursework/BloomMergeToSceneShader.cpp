#include "BloomMergeToSceneShader.h"
#include "ShaderHelpers.h"

BloomMergeToSceneShader::BloomMergeToSceneShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"general_vs.cso", L"BloomMergeToScene_ps.cso");
}


BloomMergeToSceneShader::~BloomMergeToSceneShader()
{
}

void BloomMergeToSceneShader::setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & orthoMatrix, const BloomMergeToSceneShaderParams & bloomMergeToSceneShaderParams)
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
		//Texture to merge
		deviceContext->PSSetShaderResources(0, 1, &bloomMergeToSceneShaderParams.textureToMerge);
		deviceContext->PSSetShaderResources(1, 1, &bloomMergeToSceneShaderParams.sceneTexture);

		//Sampler
		deviceContext->PSSetSamplers(0, 1, &psSamplerState);
	}

}

void BloomMergeToSceneShader::initShader(const wchar_t * vsPath, const wchar_t * psPath)
{
	/*Load VS and PS.*/
	loadVertexShader(vsPath);
	loadPixelShader(psPath);

	/*Setup VS.*/
	ShaderHelpers::setUpConstantBuffer<MatrixBufferType>(renderer, &vsMatrixBuffer);

	/*Setup PS.*/
	//ShaderHelpers::setUpConstantBuffer<BloomMergeBufferType>(renderer, &psBlurWeightsBuffer);
	ShaderHelpers::setUpSampleState(renderer, &psSamplerState);
}
