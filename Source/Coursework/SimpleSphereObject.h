#pragma once
#include "GameObject.h"

#include <array>
/// <summary>
/// An object which draws a sphere with a texture.
/// </summary>
class SimpleSphereObject :
	public GameObject
{
	std::unique_ptr<SphereMesh> sphereMesh;
	std::array<float, 3> position;
public:
	SimpleSphereObject();
	~SimpleSphereObject();

	/// <summary>
	/// Initalise mesh and load texture.
	/// </summary>
	void init() override;

	/// <summary>
	/// Update positons.
	/// </summary>
	void update() override;

	/// <summary>
	/// Show position movement.
	/// </summary>
	void gui() override;

	/// <summary>
	/// Render the depth of the mesh.
	/// </summary>
	void renderDepth(Light* light, bool isDynamic) override;

	/// <summary>
	/// Render the object with shadows.
	/// </summary>
	/// <param name="ssp"></param>
	void renderShadows(ShadowShaderParams& ssp) override;
};

