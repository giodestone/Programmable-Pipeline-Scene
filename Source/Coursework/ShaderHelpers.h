#pragma once
#include "DXF.h"
#include "BaseShader.h"

/// <summary>
/// Some helper functions for shaders.
/// </summary>
namespace ShaderHelpers
{
	
	/// <summary>
	/// Sets up a constant buffer with DataType.
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="bufferToMapTo"></param>
	template <class DataType>
	static void setUpConstantBuffer(ID3D11Device* renderer, ID3D11Buffer** bufferToMapTo)
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(DataType);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		auto result = renderer->CreateBuffer(&bufferDesc, NULL, bufferToMapTo);
		assert(!FAILED(result)); // creating buffer went wrong.
	}

	/// <summary>
	/// Sets up the samplerToMap to default optiopns.
	/// </summary>
	/// <remarks>
	///	Assertion to check if creation was succesful.
	/// </remarks>
	/// <param name="renderer"></param>
	/// <param name="samplerToMap">Sampler to setup.</param>
	/// <param name="addressU">Texture Address Mode U</param>
	/// <param name="addressV">Texture Address Mode V</param>
	/// <param name="addressW">Texture Address Mode W</param>
	static void setUpSampleState(ID3D11Device* renderer, ID3D11SamplerState** samplerToMap, 
		D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_WRAP, 
		D3D11_TEXTURE_ADDRESS_MODE addressW = D3D11_TEXTURE_ADDRESS_WRAP)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		
		// Create a texture sampler state description.
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = addressU;
		samplerDesc.AddressV = addressV;
		samplerDesc.AddressW = addressW;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		auto result = renderer->CreateSamplerState(&samplerDesc, samplerToMap);
		assert(!FAILED(result));  // creating sampler went wrong.
	}

	/// <summary>
	/// Defines alpha blending for the device.
	/// </summary>
	static void setAlphaEnabled(D3D* renderer, bool isEnabled)
	{
		ID3D11BlendState* blendState;
		D3D11_BLEND_DESC blendDesc;

		float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
		UINT sampleMask = 0xffffffff;

		blendDesc.AlphaToCoverageEnable = isEnabled;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		renderer->getDevice()->CreateBlendState(&blendDesc, &blendState);
		renderer->getDeviceContext()->OMSetBlendState(blendState, blendFactor, sampleMask);

		renderer->setAlphaBlending(isEnabled);
	}

	
	/// <summary>
	/// Sending matrices to the vertex shader is very common, so this class makes it easier.
	/// </summary>
	/// <param name="deviceContext"></param>
	/// <param name="vsMatrixBuffer"></param>
	/// <param name="slot"></param>
	/// <param name="worldMatrix"></param>
	/// <param name="viewMatrix"></param>
	/// <param name="projectionMatrix"></param>
	template <class MatrixBufferType>
	static void sendMatrixBufferToVS(ID3D11DeviceContext* deviceContext, ID3D11Buffer** vsMatrixBuffer, int slot, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		XMMATRIX tworld = XMMatrixTranspose(worldMatrix), tview = XMMatrixTranspose(viewMatrix), tproj = XMMatrixTranspose(projectionMatrix);

		auto result = deviceContext->Map(*vsMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		assert(!FAILED(result)); // Throw assert if something goes wrong.
		MatrixBufferType* matrixBufferDataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

		matrixBufferDataPtr->world = tworld;
		matrixBufferDataPtr->view = tview;
		matrixBufferDataPtr->projection = tproj;

		deviceContext->Unmap(*vsMatrixBuffer, 0);
		deviceContext->VSSetConstantBuffers(slot, 1, vsMatrixBuffer);
	}
}