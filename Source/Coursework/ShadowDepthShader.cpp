#include "ShadowDepthShader.h"
#include "ShaderHelpers.h"

ShadowDepthShader::ShadowDepthShader(ID3D11Device* device, HWND hwnd) 
	: BaseShader(device, hwnd)
	, vsMatrixBuffer(nullptr)
{
	initShader(L"shadowDepth_vs.cso", L"shadowDepth_ps.cso");
}


ShadowDepthShader::~ShadowDepthShader()
{
}

void ShadowDepthShader::setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, const ShadowDepthShaderParams & shadowDepthShaderParams)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix), tview = XMMatrixTranspose(viewMatrix), tproj = XMMatrixTranspose(projectionMatrix);

	/*Send data to VS.*/
	{
		HRESULT matrixBufferResult = deviceContext->Map(vsMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		MatrixBufferType* matrixBufferPtr = (MatrixBufferType*)mappedResource.pData;

		matrixBufferPtr->world = tworld;
		matrixBufferPtr->view = tview;
		matrixBufferPtr->projection = tproj;

		deviceContext->Unmap(vsMatrixBuffer, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &vsMatrixBuffer);
	}
}

void ShadowDepthShader::initShader(const wchar_t * vsPath, const wchar_t * psPath)
{
	loadVertexShader(vsPath);
	loadPixelShader(psPath);

	ShaderHelpers::setUpConstantBuffer<MatrixBufferType>(renderer, &vsMatrixBuffer);
}
