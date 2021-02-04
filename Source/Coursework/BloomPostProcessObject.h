#pragma once
#include "GameObject.h"

class BloomPostProcessObject :
	public GameObject
{
	std::unique_ptr<RenderTexture> sceneRenderTexture;
	std::unique_ptr<RenderTexture> sceneBrightestPartsRenderTexture;
	std::unique_ptr<RenderTexture> sceneBrightestPartsBlurredHorizontally;
	std::unique_ptr<RenderTexture> sceneBrightestPartsBlurredVertically;

	std::unique_ptr<OrthoMesh> screenSizeOrthoMesh;

	bool isEffectEnabled = true;
	bool overrideWireframeMode = true;
	float intensityCutoff = 0.275f;
	float intensityModifier = 2.f;

	///TODO MERGE BLOOM INTO TEXTURE AND RENDER TO SCREEN!

public:
	/// <summary>
	/// Is the bloom effect enabled.
	/// </summary>
	bool getIsEffectEnabled() const;

	BloomPostProcessObject();
	~BloomPostProcessObject();

	/// <summary>
	/// Initialise ortho mesh and various render textures.
	/// </summary>
	void init() override;

	/// <summary>
	/// Begin rendering to the render texture.
	/// </summary>
	void beginRenderToTexture();

	/// <summary>
	/// Finish rendering to the render texture.
	/// </summary>
	void endRenderToTexture();

	/// <summary>
	/// Get the brighest parts of the scene applying the intensity cutoff to cut off darker parts (so not everything is bloomed).
	/// </summary>
	void getBrightestPartsPass();

	/// <summary>
	/// Blur scene horizontally.
	/// </summary>
	void horizontalBlurPass();

	/// <summary>
	/// Blur scene vertically.
	/// </summary>
	void verticalBlurPass();

	/// <summary>
	/// Render to an othro mesh on top of the scene. This is done intead of the render texture as it requires less shaders.
	/// </summary>
	void mergeBloomIntoScene();

	/// <summary>
	/// Render the bloom post proccess effect.
	/// </summary>
	void gui() override;
};

