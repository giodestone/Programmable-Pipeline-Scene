#pragma once

#include <vector>
#include <memory>

/// <summary>
/// Parameters for the shadow shader.
/// </summary>
struct ShadowShaderParams
{
	std::vector<std::shared_ptr<class LightObject>> directionalLights;
	std::vector<std::shared_ptr<class LightObject>> pointLights;
	std::vector<std::shared_ptr<class LightObject>> spotLights;
	
	ID3D11ShaderResourceView* objectTexture;
	std::vector<ID3D11ShaderResourceView*> depthMaps;

	const float shadowBias = 0.0025f;
};