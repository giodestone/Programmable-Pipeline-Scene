#pragma once
#include "GameObject.h"

#include "LightObject.h"

/// <summary>
/// A wrapper around the shadow effect.
/// </summary>
class ShadowObject :
	public GameObject
{
	std::vector< std::unique_ptr<ShadowMap> > directionalShadowMaps;
	std::vector< std::shared_ptr<LightObject> > directionalLights;

	std::vector< std::shared_ptr<LightObject> > pointLights;
	std::vector< std::shared_ptr<LightObject> > spotLights;
	
	static constexpr int ShadowMapSize = 4096;

	bool areLightsSorted = false;
	
public:
	ShadowObject();
	~ShadowObject();

	/// <summary>
	/// Run scene objects through a depth pass, and sort them into point, spot, and directional lights as needed.
	/// </summary>
	/// <remarks>Doesn't support runtime initialisation.</remarks>
	/// <param name="gameObjectManager"></param>
	/// <param name="lights"></param>
	void depthPassObjects(class GameObjectManager* gameObjectManager, std::vector<std::shared_ptr<LightObject>> lights);

	/// <summary>
	/// Render the scene with shadows.
	/// </summary>
	/// <param name="gameObjectManager"></param>
	/// <param name="lights"></param>
	void renderWithShadows(class GameObjectManager* gameObjectManager, std::vector<std::shared_ptr<LightObject>> lights);

protected:
	/// <summary>
	/// Sort the lights into arrays and setup depth maps for any directional ones.
	/// </summary>
	/// <param name="lights"></param>
	void sortLights(std::vector<std::shared_ptr<LightObject>> lights);
};