#pragma once
#include "GameObject.h"
#include "DXF.h"
#include "Light.h"
#include <memory>
#include <array>
#include <string>

enum LIGHT_TYPE
{
	LT_NONE,
	LT_DIRECTIONAL,
	LT_POINT,
	LT_SPOT
};

/// <summary>
/// Wrapper around the Light class in order to to provide some extra functionality such as GUI.
/// </summary>
class LightObject :
	public GameObject
{
	LIGHT_TYPE lightType;
	
	std::unique_ptr<Light> light;
	std::array<float, 3> positionGUI; // for GUI
	std::array<float, 3> directionGUI; // for GUI
	std::array<float, 3> diffuseGUI;
	std::string name; // for GUI
	bool isDebugSphereEnabled = false;

	std::unique_ptr<SphereMesh> debugSphere;
	
public:
	LightObject();
	~LightObject();

	/// <summary>
	/// Set arrays for gui to initial values from transform.
	/// </summary>
	void init() override;

	/// <summary>
	/// Show position and direction paramaters on GUI.
	/// </summary>
	void gui() override;

	/// <summary>
	/// Update transform from GUI arrays.
	/// </summary>
	void update() override;

	/// <summary>
	/// Render the debug ball (NOT INTENDED TO BE PART OF SCENE)
	/// </summary>
	/// <param name="ssp"></param>
	void renderShadows(ShadowShaderParams& ssp) override;

	/// <summary>
	/// Get the underlying light type.
	/// </summary>
	/// <returns></returns>
	Light* getLight() { return light.get(); }

	/// <summary>
	/// Set the name that will be displayed in GUI.
	/// </summary>
	/// <param name="name"></param>
	void setName(std::string name) { this->name = name; }

	/// <summary>
	/// Set the type of this light (how it gets treated in shadow object)
	/// </summary>
	/// <param name="lightType"></param>
	void setLightType(LIGHT_TYPE lightType) { this->lightType = lightType; }

	/// <summary>
	/// Get which type this light is.
	/// </summary>
	/// <returns></returns>
	LIGHT_TYPE getLightType() const { return lightType; }
};

