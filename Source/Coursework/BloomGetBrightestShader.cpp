#include "BloomGetBrightestShader.h"
#include "ShaderHelpers.h"


BloomGetBrightestShader::BloomGetBrightestShader(ID3D11Device* device, HWND hwnd)
	: BaseShader(device, hwnd)
{
	initShader(L"general_vs.cso", L"bloomGetBrightest_ps.cso");
}


BloomGetBrightestShader::~BloomGetBrightestShader()
{
}

void BloomGetBrightestShader::initShader(const wchar_t * vsPath, const wchar_t * psPath)
{
	/*Load VS and PS*/
	loadVertexShader(vsPath);
	loadPixelShader(psPath);

	/*Setup VS.*/
	ShaderHelpers::setUpConstantBuffer<MatrixBufferType>(renderer, &vsMatrixBuffer);

	/*Setup PS.*/
	ShaderHelpers::setUpSampleState(renderer, &psSamplerState);
	ShaderHelpers::setUpConstantBuffer<BloomBrightnessBufferType>(renderer, &psBrightnessBuffer);
}

void BloomGetBrightestShader::setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & orthoMatrix, const BloomGetBrightestShaderParams & bloomBrightnessParams)
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
		//Brightness buffer
		result = deviceContext->Map(psBrightnessBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		BloomBrightnessBufferType* boomBrightnessBuffer = (BloomBrightnessBufferType*)mappedResource.pData;
		
		boomBrightnessBuffer->intensityCutOff = bloomBrightnessParams.intensityCutOff;

		deviceContext->Unmap(psBrightnessBuffer, 0);
		deviceContext->PSSetConstantBuffers(0, 1, &psBrightnessBuffer);
		
		//Scene texture
		deviceContext->PSSetShaderResources(0, 1, &bloomBrightnessParams.sceneTexture);

		//Sampler
		deviceContext->PSSetSamplers(0, 1, &psSamplerState);
	}
}
