#include "ShadowTessellationDepthShader.h"
#include "ShaderHelpers.h"


ShadowTessellationDepthShader::ShadowTessellationDepthShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tessellation_vs.cso", L"tessellation_hs.cso", L"tessellationShadowDepth_ds.cso", L"shadowDepth_ps.cso");
}


ShadowTessellationDepthShader::~ShadowTessellationDepthShader()
{
}

void ShadowTessellationDepthShader::setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, const PlaneTessellationParams & planeTessellationParams)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	XMMATRIX tworld = XMMatrixTranspose(worldMatrix), tview = XMMatrixTranspose(viewMatrix), tproj = XMMatrixTranspose(projectionMatrix);

	/*Send data to hull shader.*/
	{
		result = deviceContext->Map(hsTessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		TessellationBufferType* tesselBufferDataPtr = (TessellationBufferType*)mappedResource.pData;

		tesselBufferDataPtr->nearDistance = planeTessellationParams.nearDistance;
		tesselBufferDataPtr->farDistance = planeTessellationParams.farDistance;
		tesselBufferDataPtr->nearLOD = planeTessellationParams.nearLOD;
		tesselBufferDataPtr->farLOD = planeTessellationParams.farLOD;
		tesselBufferDataPtr->cameraPosition = planeTessellationParams.cameraPosition;

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

		heightmapParamsBufferDataPtr->amplitude = planeTessellationParams.amplitude;
		heightmapParamsBufferDataPtr->heightmapDimensions = planeTessellationParams.heightmapDimensions;

		deviceContext->Unmap(dsHeightmapParamsBuffer, 0);
		deviceContext->DSSetConstantBuffers(1, 1, &dsHeightmapParamsBuffer);

		//Heightmap texture
		deviceContext->DSSetShaderResources(0, 1, &planeTessellationParams.heightmap);

		//Sampler
		deviceContext->DSSetSamplers(0, 1, &dsHeightMapSamplerState);
	}
}

void ShadowTessellationDepthShader::initShader(const wchar_t *, const wchar_t *)
{
	assert(false);
	//Don't use this function! The VS, DS, HS, and PS need to be loaded, this only loads 2. This is exclusively to satisfy the abstract function override.
}

void ShadowTessellationDepthShader::initShader(const wchar_t * vsPath, const wchar_t * hsPath, const wchar_t * dsPath, const wchar_t * psPath)
{
	loadVertexShader(vsPath);
	loadHullShader(hsPath);
	loadDomainShader(dsPath);
	loadPixelShader(psPath);

	/*Setup Hull Shader*/
	ShaderHelpers::setUpConstantBuffer<TessellationBufferType>(renderer, &hsTessellationBuffer);

	/*Setup Domain Shader*/
	ShaderHelpers::setUpConstantBuffer<MatrixBufferType>(renderer, &dsMatrixBuffer);
	ShaderHelpers::setUpConstantBuffer<HeightmapParamsBufferType>(renderer, &dsHeightmapParamsBuffer);
	ShaderHelpers::setUpSampleState(renderer, &dsHeightMapSamplerState);
}
