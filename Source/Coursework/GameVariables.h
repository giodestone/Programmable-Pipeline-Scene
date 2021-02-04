#pragma once

#include <memory>

#include "DXF.h"

#include "SimpleShader.h"
#include "PlaneTessellationShader.h"

#include "BloomGetBrightestShader.h"
#include "BloomHorizontalBlurShader.h"
#include "BloomVerticalBlurShader.h"
#include "BloomMergeToSceneShader.h"

#include "ShadowDepthShader.h"
#include "ShadowShader.h"
#include "ShadowTessellationDepthShader.h"
#include "ShadowTessellationShader.h"

class App1; // Forward declare App1.

/// <summary>
/// For storing references to shaders and other useful game variables for each game object class.
/// </summary>
class GameVariables
{
private:
	friend class App1; //So app 1 can set these values and no other classes mess with them.
	
	/*Variables*/
	TextureManager* textureManager;
	Input* input;
	Timer* timer;
	Camera* mainCamera;
	D3D* renderer;
	HWND hwnd;
	float screenNear, screenDepth;
	XMFLOAT2 screenDimensions;

	/*Shaders*/
	std::unique_ptr<SimpleShader> simpleShader;
	std::unique_ptr<PlaneTessellationShader> planeTessellationShader;
	std::unique_ptr<BloomGetBrightestShader> bloomGetBrightestShader;
	std::unique_ptr<BloomHorizontalBlurShader> bloomHorizontalBlurShader;
	std::unique_ptr<BloomVerticalBlurShader> bloomVerticalBlurShader;
	std::unique_ptr<BloomMergeToSceneShader> bloomMergeToSceneShader;
	std::unique_ptr<ShadowDepthShader> shadowDepthShader;
	std::unique_ptr<ShadowShader> shadowShader;
	std::unique_ptr<ShadowTessellationDepthShader> shadowTessellationDepthShader;
	std::unique_ptr<ShadowTessellationShader> shadowTessellationShader;

	/*Functions*/
	void initShaders()
	{
		simpleShader = std::make_unique<SimpleShader>(renderer->getDevice(), hwnd);
		planeTessellationShader = std::make_unique<PlaneTessellationShader>(renderer->getDevice(), hwnd);
		bloomGetBrightestShader = std::make_unique<BloomGetBrightestShader>(renderer->getDevice(), hwnd);
		bloomHorizontalBlurShader = std::make_unique<BloomHorizontalBlurShader>(renderer->getDevice(), hwnd);
		bloomVerticalBlurShader = std::make_unique<BloomVerticalBlurShader>(renderer->getDevice(), hwnd);
		bloomMergeToSceneShader = std::make_unique<BloomMergeToSceneShader>(renderer->getDevice(), hwnd);
		shadowDepthShader = std::make_unique<ShadowDepthShader>(renderer->getDevice(), hwnd);
		shadowShader = std::make_unique<ShadowShader>(renderer->getDevice(), hwnd);
		shadowTessellationDepthShader = std::make_unique<ShadowTessellationDepthShader>(renderer->getDevice(), hwnd);
		shadowTessellationShader = std::make_unique<ShadowTessellationShader>(renderer->getDevice(), hwnd);
	}

public:
#pragma region Getters
	TextureManager* getTextureManager() const
	{
		assert(textureManager != nullptr);
		return textureManager;
	}

	Input* getInput() const
	{
		assert(input != nullptr);
		return input;
	}

	Timer* getTimer() const
	{
		assert(timer != nullptr);
		return timer;
	}

	Camera* getMainCamera() const
	{
		assert(mainCamera != nullptr);
		return mainCamera;
	}

	D3D* getRenderer() const
	{
		assert(renderer != nullptr);
		return renderer;
	}

	HWND getHWND() const
	{
		assert(hwnd != NULL);
		return hwnd;
	}

	float getScreenNear() const
	{
		assert(screenNear > 0.f); //This check isnt the best... uninitialised variables will just result in weird values.
		return screenNear;
	}

	float getScreenDepth() const
	{
		assert(screenDepth > 0.f); //This check isnt the best either... uninitialised variables will just result in weird values.
		return screenDepth;
	}

	XMFLOAT2 getScreenDimensions() const
	{
		assert(screenDimensions.x > 0.f && screenDimensions.y > 0.f); //Or this check... Better than nothing I guess.
		return screenDimensions;
	}
#pragma endregion

#pragma region Shaders
	SimpleShader* getSimpleShader()
	{
		assert(simpleShader.get() != nullptr);
		return simpleShader.get();
	}

	PlaneTessellationShader* getPlaneTessellationShader()
	{
		assert(planeTessellationShader.get() != nullptr);
		return planeTessellationShader.get();
	}

	BloomGetBrightestShader* getBloomGetBrightestShader()
	{
		assert(bloomGetBrightestShader.get() != nullptr);
		return bloomGetBrightestShader.get();
	}

	BloomHorizontalBlurShader* getBloomHorizontalBlurShader()
	{
		assert(bloomHorizontalBlurShader.get() != nullptr);
		return bloomHorizontalBlurShader.get();
	}

	BloomVerticalBlurShader* getBloomVerticalBlurShader()
	{
		assert(bloomVerticalBlurShader.get() != nullptr);
		return bloomVerticalBlurShader.get();
	}

	BloomMergeToSceneShader* getBloomMergeToSceneShader()
	{
		assert(bloomMergeToSceneShader.get() != nullptr);
		return bloomMergeToSceneShader.get();
	}

	ShadowDepthShader* getShadowDepthShader()
	{
		assert(shadowDepthShader.get() != nullptr);
		return shadowDepthShader.get();
	}

	ShadowShader* getShadowShader()
	{
		assert(shadowShader.get() != nullptr);
		return shadowShader.get();
	}
	
	ShadowTessellationDepthShader* getShadowTessellationDepthShader()
	{
		assert(shadowTessellationDepthShader.get() != nullptr);
		return shadowTessellationDepthShader.get();
	}
	
	ShadowTessellationShader* getShadowTessellationShader()
	{
		assert(shadowTessellationShader.get() != nullptr);
		return shadowTessellationShader.get();
	}
#pragma endregion
};