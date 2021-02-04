#include "BloomPostProcessObject.h"
#include "GameObjectManager.h"
#include "ShaderHelpers.h"

bool BloomPostProcessObject::getIsEffectEnabled() const
{
	return isEffectEnabled;
}

BloomPostProcessObject::BloomPostProcessObject()
	: GameObject(GAME_OBJECT_TYPE::GOT_BLOOM_POST_PROCESS)
{
}


BloomPostProcessObject::~BloomPostProcessObject()
{
}

void BloomPostProcessObject::init()
{
	/*Make scene render textures.*/
	sceneRenderTexture = std::make_unique<RenderTexture>(getGameVars()->getRenderer()->getDevice(),
		static_cast<int>(getGameVars()->getScreenDimensions().x), static_cast<int>(getGameVars()->getScreenDimensions().y),
		getGameVars()->getScreenNear(), getGameVars()->getScreenDepth());
	sceneBrightestPartsRenderTexture = std::make_unique<RenderTexture>(getGameVars()->getRenderer()->getDevice(),
		static_cast<int>(getGameVars()->getScreenDimensions().x), static_cast<int>(getGameVars()->getScreenDimensions().y),
		getGameVars()->getScreenNear(), getGameVars()->getScreenDepth());
	sceneBrightestPartsBlurredHorizontally = std::make_unique<RenderTexture>(getGameVars()->getRenderer()->getDevice(),
		static_cast<int>(getGameVars()->getScreenDimensions().x), static_cast<int>(getGameVars()->getScreenDimensions().y),
		getGameVars()->getScreenNear(), getGameVars()->getScreenDepth());
	sceneBrightestPartsBlurredVertically = std::make_unique<RenderTexture>(getGameVars()->getRenderer()->getDevice(),
		static_cast<int>(getGameVars()->getScreenDimensions().x), static_cast<int>(getGameVars()->getScreenDimensions().y),
		getGameVars()->getScreenNear(), getGameVars()->getScreenDepth());

	/*Create ortho mesh size of screen.*/
	screenSizeOrthoMesh = std::make_unique<OrthoMesh>(getGameVars()->getRenderer()->getDevice(), getGameVars()->getRenderer()->getDeviceContext(),
		static_cast<int>(getGameVars()->getScreenDimensions().x), static_cast<int>(getGameVars()->getScreenDimensions().y));
}

void BloomPostProcessObject::beginRenderToTexture()
{
	sceneRenderTexture->setRenderTarget(getGameVars()->getRenderer()->getDeviceContext());
	sceneRenderTexture->clearRenderTarget(getGameVars()->getRenderer()->getDeviceContext(), 0.f, 0.f, 0.f, 0.f);
}

void BloomPostProcessObject::endRenderToTexture()
{
	getGameVars()->getRenderer()->setBackBufferRenderTarget();
}

void BloomPostProcessObject::getBrightestPartsPass()
{
	/*Get brightest parts to texture.*/
	sceneBrightestPartsRenderTexture->setRenderTarget(getGameVars()->getRenderer()->getDeviceContext());
	sceneBrightestPartsRenderTexture->clearRenderTarget(getGameVars()->getRenderer()->getDeviceContext(), 0.f, 0.f, 0.f, 1.f);

	BloomGetBrightestShaderParams bgbsp;
	bgbsp.intensityCutOff = intensityCutoff;
	bgbsp.sceneTexture = sceneRenderTexture->getShaderResourceView();

	screenSizeOrthoMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());

	getGameVars()->getBloomGetBrightestShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		getGameVars()->getRenderer()->getWorldMatrix(),
		getGameVars()->getMainCamera()->getOrthoViewMatrix(),
		getGameVars()->getRenderer()->getOrthoMatrix(),
		bgbsp);

	getGameVars()->getBloomGetBrightestShader()->render(getGameVars()->getRenderer()->getDeviceContext(), screenSizeOrthoMesh->getIndexCount());

	getGameVars()->getRenderer()->setBackBufferRenderTarget();
}

void BloomPostProcessObject::horizontalBlurPass()
{
	/*Blur horizontally*/
	sceneBrightestPartsBlurredHorizontally->setRenderTarget(getGameVars()->getRenderer()->getDeviceContext());
	sceneBrightestPartsBlurredHorizontally->clearRenderTarget(getGameVars()->getRenderer()->getDeviceContext(), 0.f, 0.f, 0.f, 1.f);

	BloomHorizontalBlurShaderParams bhbsp;
	bhbsp.textureDimensions = getGameVars()->getScreenDimensions();
	bhbsp.textureToBlurHorizontally = sceneBrightestPartsRenderTexture->getShaderResourceView();
	bhbsp.intensityModifier = intensityModifier;

	screenSizeOrthoMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());

	getGameVars()->getBloomHorizontalBlurShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		getGameVars()->getRenderer()->getWorldMatrix(),
		getGameVars()->getMainCamera()->getOrthoViewMatrix(),
		getGameVars()->getRenderer()->getOrthoMatrix(),
		bhbsp);
	getGameVars()->getBloomHorizontalBlurShader()->render(getGameVars()->getRenderer()->getDeviceContext(), screenSizeOrthoMesh->getIndexCount());

	getGameVars()->getRenderer()->setBackBufferRenderTarget();
}

void BloomPostProcessObject::verticalBlurPass()
{
	/*Blur vertically*/
	sceneBrightestPartsBlurredVertically->setRenderTarget(getGameVars()->getRenderer()->getDeviceContext());
	sceneBrightestPartsBlurredVertically->clearRenderTarget(getGameVars()->getRenderer()->getDeviceContext(), 0.f, 0.f, 0.f, 1.f);

	BloomVeritcalBlurShaderParams bvbsp;
	bvbsp.textureDimensions = getGameVars()->getScreenDimensions();
	bvbsp.textureToBlur = sceneBrightestPartsBlurredHorizontally->getShaderResourceView();
	bvbsp.intensityModifier = intensityModifier;

	screenSizeOrthoMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());

	getGameVars()->getBloomVerticalBlurShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		getGameVars()->getRenderer()->getWorldMatrix(),
		getGameVars()->getMainCamera()->getOrthoViewMatrix(),
		getGameVars()->getRenderer()->getOrthoMatrix(),
		bvbsp);
	getGameVars()->getBloomVerticalBlurShader()->render(getGameVars()->getRenderer()->getDeviceContext(), screenSizeOrthoMesh->getIndexCount());

	getGameVars()->getRenderer()->setBackBufferRenderTarget();
}

void BloomPostProcessObject::mergeBloomIntoScene()
{
	if (isEffectEnabled)
	{
		bool wasWireframeEnabled = false;
		if (overrideWireframeMode && getGameVars()->getRenderer()->getWireframeState()) //if wireframe enabled
		{
			getGameVars()->getRenderer()->setWireframeMode(false);
			wasWireframeEnabled = true;
		}

		/*Merge the render into the general scene.*/
		getGameVars()->getRenderer()->setZBuffer(false);

		BloomMergeToSceneShaderParams bmtssp;
		bmtssp.textureToMerge = sceneBrightestPartsBlurredVertically->getShaderResourceView();
		bmtssp.sceneTexture = sceneRenderTexture->getShaderResourceView();
		screenSizeOrthoMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());
		getGameVars()->getBloomMergeToSceneShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
			getGameVars()->getRenderer()->getWorldMatrix(),
			getGameVars()->getMainCamera()->getOrthoViewMatrix(),
			getGameVars()->getRenderer()->getOrthoMatrix(),
			bmtssp);
		getGameVars()->getBloomMergeToSceneShader()->render(getGameVars()->getRenderer()->getDeviceContext(), screenSizeOrthoMesh->getIndexCount());

		getGameVars()->getRenderer()->setZBuffer(true);

		if (wasWireframeEnabled) // Enable wireframe again after it was disabled.
			getGameVars()->getRenderer()->setWireframeMode(true);
	}
}

void BloomPostProcessObject::gui()
{
	/*Add collapsing header for the GUI options.*/
	if (ImGui::CollapsingHeader("Bloom Post Process"))
	{
		ImGui::Checkbox("Enabled", &isEffectEnabled);
		ImGui::Checkbox("Override Wireframe", &overrideWireframeMode);
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Whether to override wireframe when rendering the screen ortho mesh.");
		}

		ImGui::DragFloat("Bloom Cut off", &intensityCutoff, 0.001f, 0.f, 1.f);
		ImGui::DragFloat("Intensity Modifier", &intensityModifier, 0.001f, 0.f, 100.f);
	}
}
