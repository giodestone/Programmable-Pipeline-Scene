#include "ShadowTessellationShader.h"
#include "ShaderHelpers.h"
#include "LightObject.h"

ShadowTessellationShader::ShadowTessellationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tessellation_vs.cso", L"tessellation_hs.cso", L"tessellationShadow_ds.cso", L"shadow_ps.cso");
}


ShadowTessellationShader::~ShadowTessellationShader()
{
}

void ShadowTessellationShader::setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, const PlaneTessellationParams & planeTessellationParams, const ShadowShaderParams & shadowShaderParams)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	XMMATRIX tworld = XMMatrixTranspose(worldMatrix), tview = XMMatrixTranspose(viewMatrix), tproj = XMMatrixTranspose(projectionMatrix);

	/*Send tessellation data to hull shader.*/
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

		//Light matrix buffer
		result = deviceContext->Map(dsLightMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		LightMatrixBufferType* lightMatrixBufferDataPtr = static_cast<LightMatrixBufferType*>(mappedResource.pData);

		assert(shadowShaderParams.directionalLights.size() == NO_OF_SHADOWED_LIGHTS); // The amount of directional lights do not equal the macro! Update the macro in Shadow Shader Types, shadow_vs, shadow_ps, and tessellation_ds!
		for (int i = 0; i < NO_OF_SHADOWED_LIGHTS; i++)
		{
			lightMatrixBufferDataPtr->lightView[i] = XMMatrixTranspose(shadowShaderParams.directionalLights[i]->getLight()->getViewMatrix());
			lightMatrixBufferDataPtr->lightProjection[i] = XMMatrixTranspose(shadowShaderParams.directionalLights[i]->getLight()->getOrthoMatrix());
		}
		deviceContext->Unmap(dsLightMatrixBuffer, 0);
		deviceContext->DSSetConstantBuffers(2, 1, &dsLightMatrixBuffer);

		// Heightmap texture
		deviceContext->DSSetShaderResources(0, 1, &planeTessellationParams.heightmap);

		// Sampler
		deviceContext->DSSetSamplers(0, 1, &dsHeightMapSamplerState);
	}

	/*Send data to PS*/
	{
		/*Light and shadow buffer.*/
		result = deviceContext->Map(psLightAndShadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		LightAndShadowBufferType* lightAndShadowBufferDataPtr = static_cast<LightAndShadowBufferType*>(mappedResource.pData);

		lightAndShadowBufferDataPtr->ambient = shadowShaderParams.directionalLights.front()->getLight()->getAmbientColour();
		lightAndShadowBufferDataPtr->shadowMapBias = shadowShaderParams.shadowBias;

		deviceContext->Unmap(psLightAndShadowBuffer, 0);
		deviceContext->PSSetConstantBuffers(0, 1, &psLightAndShadowBuffer);

		
		/*Directional Light Buffer*/
		result = deviceContext->Map(psDirectionalLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		DirectionalLightBufferType* directionalLightBufferDataPtr = static_cast<DirectionalLightBufferType*>(mappedResource.pData);
		
		assert(shadowShaderParams.directionalLights.size() == NO_OF_SHADOWED_LIGHTS); // The amount of directional lights do not equal the macro! Update the macro in Shadow Shader Types, shadow_vs, shadow_ps, and tessellation_ds!
		for (int i = 0; i < NO_OF_SHADOWED_LIGHTS; i++)
		{
			directionalLightBufferDataPtr->diffuse[i] = shadowShaderParams.directionalLights[i]->getLight()->getDiffuseColour();
			directionalLightBufferDataPtr->direction[i] = XMFLOAT4(shadowShaderParams.directionalLights[i]->getLight()->getDirection().x, shadowShaderParams.directionalLights[i]->getLight()->getDirection().y, shadowShaderParams.directionalLights[i]->getLight()->getDirection().z, 0.f);
		}

		deviceContext->Unmap(psDirectionalLightBuffer, 0);
		deviceContext->PSSetConstantBuffers(1, 1, &psDirectionalLightBuffer);

		
		/*Spot light buffer*/
		result = deviceContext->Map(psPointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		auto* pointLightBufferDataPtr = static_cast<PointLightBufferType*>(mappedResource.pData);

		assert(NO_OF_POINT_LIGHTS == shadowShaderParams.pointLights.size());// The amount of point lights do not equal the macro! Update the macro in Shadow Shader Types, shadow_vs, shadow_ps, and tessellation_ds!
		for (size_t i = 0; i < NO_OF_POINT_LIGHTS; i++)
		{
			pointLightBufferDataPtr->diffuse[i] = shadowShaderParams.pointLights[i]->getLight()->getDiffuseColour();
			pointLightBufferDataPtr->position[i] = XMFLOAT4(shadowShaderParams.pointLights[i]->getLight()->getPosition().x, shadowShaderParams.pointLights[i]->getLight()->getPosition().y, shadowShaderParams.pointLights[i]->getLight()->getPosition().z, 1.f);
		}

		deviceContext->Unmap(psPointLightBuffer, 0);
		deviceContext->PSSetConstantBuffers(2, 1, &psPointLightBuffer);


		/*Point light buffer*/
		result = deviceContext->Map(psSpotLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		auto* spotLightBufferDataPtr = static_cast<SpotLightBufferType*>(mappedResource.pData);

		assert(NO_OF_SPOT_LIGHTS == shadowShaderParams.spotLights.size());// The amount of spot lights do not equal the macro! Update the macro in Shadow Shader Types, shadow_vs, shadow_ps, and tessellation_ds!
		for (size_t i = 0; i < NO_OF_SPOT_LIGHTS; i++)
		{
			spotLightBufferDataPtr->diffuse[i] = shadowShaderParams.spotLights[i]->getLight()->getDiffuseColour();
			spotLightBufferDataPtr->position[i] = XMFLOAT4(shadowShaderParams.spotLights[i]->getLight()->getPosition().x, shadowShaderParams.spotLights[i]->getLight()->getPosition().y, shadowShaderParams.spotLights[i]->getLight()->getPosition().z, 1.f);
			spotLightBufferDataPtr->direction[i] = XMFLOAT4(shadowShaderParams.spotLights[i]->getLight()->getDirection().x, shadowShaderParams.spotLights[i]->getLight()->getDirection().y, shadowShaderParams.spotLights[i]->getLight()->getDirection().z, 0.f);
		}

		deviceContext->Unmap(psSpotLightBuffer, 0);
		deviceContext->PSSetConstantBuffers(3, 1, &psSpotLightBuffer);

		
		/*Set sampler states.*/
		deviceContext->PSSetSamplers(0, 1, &psSamplerState);
		deviceContext->PSSetSamplers(1, 1, &psSamplerState);

		/*Set textures.*/
		deviceContext->PSSetShaderResources(0, 1, &shadowShaderParams.objectTexture);

		assert(shadowShaderParams.depthMaps.size() == NO_OF_SHADOWED_LIGHTS); // The amount of depth maps do not equal the macro! Update the macro in Shadow Shader Types, shadow_vs, shadow_ps, and tessellation_ds!
		deviceContext->PSSetShaderResources(1, NO_OF_SHADOWED_LIGHTS, shadowShaderParams.depthMaps.data());
	}
}

void ShadowTessellationShader::initShader(const wchar_t *, const wchar_t *)
{
	assert(false);
	//Don't use this function! The VS, DS, HS, and PS need to be loaded, this only loads 2. This is exclusively to satisfy the abstract function override.
}

void ShadowTessellationShader::initShader(const wchar_t * vsPath, const wchar_t * hsPath, const wchar_t * dsPath, const wchar_t * psPath)
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
	ShaderHelpers::setUpConstantBuffer<LightMatrixBufferType>(renderer, &dsLightMatrixBuffer);
	ShaderHelpers::setUpSampleState(renderer, &dsHeightMapSamplerState);

	/*Pixel shader*/
	ShaderHelpers::setUpConstantBuffer<LightAndShadowBufferType>(renderer, &psLightAndShadowBuffer);
	ShaderHelpers::setUpConstantBuffer<DirectionalLightBufferType>(renderer, &psDirectionalLightBuffer);
	ShaderHelpers::setUpConstantBuffer<SpotLightBufferType>(renderer, &psSpotLightBuffer);
	ShaderHelpers::setUpConstantBuffer<PointLightBufferType>(renderer, &psPointLightBuffer);

	ShaderHelpers::setUpSampleState(renderer, &psSamplerState);
}