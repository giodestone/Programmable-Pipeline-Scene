#pragma once
#include "GameObject.h"
#include "PlaneMesh.h"


/// <summary>
/// An object that is a wrapper for a plane.
/// </summary>
class SimplePlaneObject :
	public GameObject
{
	std::unique_ptr<PlaneMesh> planeMesh;

	wchar_t* textureAlias;
	wchar_t* texturePath;

	float yPosGUI; // For GUI
public:
	SimplePlaneObject();
	~SimplePlaneObject();

	/// <summary>
	/// Set the planes' texture alias.
	/// </summary>
	/// <param name="alias"></param>
	void setTextureAlias(wchar_t* alias) { this->textureAlias = alias; }

	/// <summary>
	/// Set the planes' texture path.
	/// </summary>
	/// <param name="path"></param>
	void setTexturePath(wchar_t* path) { this->texturePath = path; }
	
protected:
	/// <summary>
	/// Setup the mesh and load texture.
	/// </summary>
	void init() override;

	/// <summary>
	/// Depth pass the frame.
	/// </summary>
	/// <param name="light"></param>
	/// <param name="isDynamic"></param>
	void renderDepth(Light* light, bool isDynamic) override;

	/// <summary>
	/// Render the shadows on the plane.
	/// </summary>
	/// <param name="ssp"></param>
	void renderShadows(ShadowShaderParams& ssp) override;

	/// <summary>
	/// Display a y position drag float.
	/// </summary>
	void gui() override;

	/// <summary>
	/// Update the transform ref from the drag float.
	/// </summary>
	void update() override;
};

