#include "ShadowShader.h"
#include "ShaderHelpers.h"
#include "LightObject.h"

ShadowShader::ShadowShader(ID3D11Device* device, HWND hwnd) 
	: BaseShader(device, hwnd)
{
	initShader(L"shadow_vs.cso", L"shadow_ps.cso");
}


ShadowShader::~ShadowShader()
{
}

void ShadowShader::setShaderParams(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projMatrix, const ShadowShaderParams & shadowShaderParams)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	XMMATRIX tworld = XMMatrixTranspose(worldMatrix), tview = XMMatrixTranspose(viewMatrix), tProj = XMMatrixTranspose(projMatrix);

	/*Send data to VS*/
	{
		/*Matrix buffer.*/
		ShaderHelpers::sendMatrixBufferToVS<MatrixBufferType>(deviceContext, &vsMatrixBuffer, 0, worldMatrix, viewMatrix, projMatrix);

		/*Light Matrix buffer.*/
		result = deviceContext->Map(vsLightMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		LightMatrixBufferType* lightMatrixBufferDataPtr = static_cast<LightMatrixBufferType*>(mappedResource.pData);

		assert(shadowShaderParams.directionalLights.size() == NO_OF_SHADOWED_LIGHTS); // The amount of directional lights do not equal the macro! Update the macro in Shadow Shader Types, shadow_vs, shadow_ps, and tessellation_ds!
		for (int i = 0; i < NO_OF_SHADOWED_LIGHTS; i++)
		{
			lightMatrixBufferDataPtr->lightView[i] = XMMatrixTranspose(shadowShaderParams.directionalLights[i]->getLight()->getViewMatrix());
			lightMatrixBufferDataPtr->lightProjection[i] = XMMatrixTranspose(shadowShaderParams.directionalLights[i]->getLight()->getOrthoMatrix());
		}
		
		deviceContext->Unmap(vsLightMatrixBuffer, 0);
		deviceContext->VSSetConstantBuffers(1, 1, &vsLightMatrixBuffer);
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
		deviceContext->PSSetShaderResources(1, NO_OF_SHADOWED_LIGHTS, shadowShaderParams.depthMaps.data());
	}
}

void ShadowShader::initShader(const wchar_t * vsPath, const wchar_t * psPath)
{
	loadVertexShader(vsPath);
	loadPixelShader(psPath);

	/*Setup buffers and sample states.*/
	ShaderHelpers::setUpConstantBuffer<MatrixBufferType>(renderer, &vsMatrixBuffer);
	ShaderHelpers::setUpConstantBuffer<LightMatrixBufferType>(renderer, &vsLightMatrixBuffer);

	ShaderHelpers::setUpConstantBuffer<LightAndShadowBufferType>(renderer, &psLightAndShadowBuffer);
	ShaderHelpers::setUpConstantBuffer<DirectionalLightBufferType>(renderer, &psDirectionalLightBuffer);
	ShaderHelpers::setUpConstantBuffer<SpotLightBufferType>(renderer, &psSpotLightBuffer);
	ShaderHelpers::setUpConstantBuffer<PointLightBufferType>(renderer, &psPointLightBuffer);
	
	ShaderHelpers::setUpSampleState(renderer, &psSamplerState);
}
