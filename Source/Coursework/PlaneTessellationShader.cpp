#include "PlaneTessellationShader.h"

#include <cassert>
#include "ShaderHelpers.h"

PlaneTessellationShader::PlaneTessellationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tessellation_vs.cso", L"tessellation_hs.cso", L"tessellation_ds.cso", L"general_ps.cso");
}


PlaneTessellationShader::~PlaneTessellationShader()
{
}

void PlaneTessellationShader::initShader(const wchar_t *, const wchar_t *)
{
	assert(false);
	//Don't use this function! The VS, DS, HS, and PS need to be loaded, this only loads 2. This is exclusively to satisfy the abstract function override.
}

void PlaneTessellationShader::initShader(const wchar_t * vs, const wchar_t * hs, const wchar_t * ds, const wchar_t * ps)
{
	loadVertexShader(vs);
	loadHullShader(hs);
	loadDomainShader(ds);
	loadPixelShader(ps);

	/*Setup Hull Shader*/
	ShaderHelpers::setUpConstantBuffer<TessellationBufferType>(renderer, &hsTessellationBuffer);
	
	/*Setup Domain Shader*/
	ShaderHelpers::setUpConstantBuffer<MatrixBufferType>(renderer, &dsMatrixBuffer);
	ShaderHelpers::setUpConstantBuffer<HeightmapParamsBufferType>(renderer, &dsHeightmapParamsBuffer);
	ShaderHelpers::setUpSampleState(renderer, &dsHeightMapSamplerState);

	/*Setup Pixel Shader*/
	ShaderHelpers::setUpSampleState(renderer, &psTextureSamplerState);
}

void PlaneTessellationShader::setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, const PlaneTessellationShaderParams & planeShaderParams)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	XMMATRIX tworld = XMMatrixTranspose(worldMatrix), tview = XMMatrixTranspose(viewMatrix), tproj = XMMatrixTranspose(projectionMatrix);

	/*Send data to hull shader.*/
	{
		result = deviceContext->Map(hsTessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		TessellationBufferType* tesselBufferDataPtr = (TessellationBufferType*)mappedResource.pData;

		tesselBufferDataPtr->minDistance = planeShaderParams.minDistance;
		tesselBufferDataPtr->maxDistance = planeShaderParams.maxDistance;
		tesselBufferDataPtr->maxLOD = planeShaderParams.maxLOD;
		tesselBufferDataPtr->minLOD = planeShaderParams.minLOD;
		tesselBufferDataPtr->cameraPosition = planeShaderParams.cameraPosition;

		deviceContext->Unmap(hsTessellationBuffer, 0);
		deviceContext->HSSetConstantBuffers(0, 1, &hsTessellationBuffer);
	}

	/*Send data to domain shader.*/
	{
		//Matrix buffer
		result = deviceContext->Map(dsMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		MatrixBufferType* matrixBufferDataPtr = (MatrixBufferType*)mappedResource.pData;

		matrixBufferDataPtr->world = tworld;
		matrixBufferDataPtr->view = tview;
		matrixBufferDataPtr->projection = tproj;

		deviceContext->Unmap(dsMatrixBuffer, 0);
		deviceContext->DSSetConstantBuffers(0, 1, &dsMatrixBuffer);

		//Heightmap params
		result = deviceContext->Map(dsHeightmapParamsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		HeightmapParamsBufferType* heightmapParamsBufferDataPtr = (HeightmapParamsBufferType*)mappedResource.pData;

		heightmapParamsBufferDataPtr->amplitude = planeShaderParams.amplitude;
		heightmapParamsBufferDataPtr->heightmapDimensions = planeShaderParams.heightmapDimensions;

		deviceContext->Unmap(dsHeightmapParamsBuffer, 0);
		deviceContext->DSSetConstantBuffers(1, 1, &dsHeightmapParamsBuffer);

		//Heightmap texture
		deviceContext->DSSetShaderResources(0, 1, &planeShaderParams.heightmap);

		//Sampler
		deviceContext->DSSetSamplers(0, 1, &dsHeightMapSamplerState);

	}

	/*Send data to pixel shader.*/
	{
		//Texture
		deviceContext->PSSetShaderResources(0, 1, &planeShaderParams.texture);

		//Sampler
		deviceContext->PSSetSamplers(0, 1, &psTextureSamplerState);
	}
}
