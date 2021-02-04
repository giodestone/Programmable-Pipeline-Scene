#pragma once
#include "GameObject.h"
#include "TessellationPlaneMesh.h"

class TessellatedPlaneObject :
	public GameObject
{
	std::unique_ptr<TessellationPlaneMesh> tessellationPlaneMesh; // Mesh for the plane

	float tessellationModifier;
	float amplitude;

	float farDistance = 30.f;
	float nearDistance = 5.f;
	float tessellationAtFar = 6.f;
	float tessellationAtNear = 32.f;
public:
	TessellatedPlaneObject();
	~TessellatedPlaneObject();

	/// <summary>
	/// Setup initial parameters, instance tessellation plane mesh.
	/// </summary>
	void init() override;

	/// <summary>
	/// Depth pass - render the tessellated terrain.
	/// </summary>
	/// <param name="light">Light to shadow against.</param>
	/// <param name="isDynamic">Whether the ortho matrix should be recalculated.</param>
	void renderDepth(Light* light, bool isDynamic) override;

	/// <summary>
	/// Shadow pass - renders the tessellated plane with shadows.
	/// </summary>
	/// <param name="shadowMap">Depth map for the light.</param>
	/// <param name="directonalLight">Light corresponding to depth map.</param>
	void renderShadows(ShadowShaderParams& ssp) override;

	/// <summary>
	/// Add parameters to the GUI.
	/// </summary>
	void gui() override;

	/// <summary>
	/// Get the plane tessellation params with values from the GUI substituted in.
	/// </summary>
	/// <returns></returns>
	PlaneTessellationParams getPlaneTessellationParams() const;
};

