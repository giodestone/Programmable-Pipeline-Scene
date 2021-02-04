#include "SimpleShader.h"

#include "ShaderHelpers.h"


SimpleShader::SimpleShader(ID3D11Device* device, HWND hwnd) 
	: BaseShader(device, hwnd)
	, VSMatrixBuffer(NULL)
	, PSSamplerState(NULL)
{
	initShader(L"general_vs.cso", L"general_ps.cso");
}


SimpleShader::~SimpleShader()
{
}

void SimpleShader::initShader(const wchar_t * vsFileName, const wchar_t * psFileName)
{
	loadVertexShader(vsFileName);
	loadPixelShader(psFileName);

	ShaderHelpers::setUpConstantBuffer<MatrixBufferType>(renderer, &VSMatrixBuffer);
	ShaderHelpers::setUpSampleState(renderer, &PSSamplerState);
}

void SimpleShader::setShaderParams(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, const SimpleShaderParams & simpleShaderParams)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	/* Pass matrices to vertex shader. */
	XMMATRIX tWorld, tView, tProj;
	tWorld = XMMatrixTranspose(worldMatrix);
	tView = XMMatrixTranspose(viewMatrix);
	tProj = XMMatrixTranspose(projectionMatrix);

	auto VSMatrixResult = deviceContext->Map(VSMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	MatrixBufferType* VSMatrixDataPtr = (MatrixBufferType*)mappedResource.pData;
	VSMatrixDataPtr->world = tWorld;
	VSMatrixDataPtr->view = tView;
	VSMatrixDataPtr->projection = tProj;
	deviceContext->Unmap(VSMatrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &VSMatrixBuffer);

	/* Pass texture and sampler to pixel shader. */
	deviceContext->PSSetShaderResources(0, 1, &simpleShaderParams.texture);
	deviceContext->PSSetSamplers(0, 1, &PSSamplerState);
}
