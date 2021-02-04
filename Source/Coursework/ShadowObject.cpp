#include "ShadowObject.h"
#include "GameObjectManager.h"

ShadowObject::ShadowObject()
{
}


ShadowObject::~ShadowObject()
{
}

void ShadowObject::depthPassObjects(GameObjectManager* gameObjectManager, std::vector<std::shared_ptr<LightObject>> lights)
{
	/*Sort all of the lights into respective arrays and generate any shadow maps for directional lights.*/
	if (!areLightsSorted)
	{
		sortLights(lights);
		areLightsSorted = true;
	}
	
	/*Render the depth of directional lights.*/
	for (int mapI = 0; mapI < NO_OF_SHADOWED_LIGHTS; ++mapI)
	{
		directionalShadowMaps.at(mapI)->BindDsvAndSetNullRenderTarget(getGameVars()->getRenderer()->getDeviceContext());

		gameObjectManager->renderDepthOfGameObjects(directionalLights.at(mapI)->getLight(), false);

		getGameVars()->getRenderer()->resetViewport();
		getGameVars()->getRenderer()->setBackBufferRenderTarget();
	}
}

void ShadowObject::renderWithShadows(GameObjectManager * gameObjectManager, std::vector<std::shared_ptr<LightObject>> lights)
{
	ShadowShaderParams ssp;
	assert(directionalLights.size() == NO_OF_SHADOWED_LIGHTS && directionalShadowMaps.size() == NO_OF_SHADOWED_LIGHTS); // Number of lights and shadow maps do not match macros. Update ShadowShaderBufferTypes.h, shadow_ps, shadow_vs, tessellatedShadow_hs!
	for (size_t mapI = 0; mapI < NO_OF_SHADOWED_LIGHTS; ++mapI)
	{
		ssp.directionalLights.emplace_back(directionalLights.at(mapI));
		ssp.depthMaps.emplace_back(directionalShadowMaps.at(mapI)->getDepthMapSRV());
	}

	/*Set references to point and spot lights.*/
	for (auto point : pointLights)
		ssp.pointLights.emplace_back(point);

	for (auto spot : spotLights)
		ssp.spotLights.emplace_back(spot);

	/*For each directional light render a shadow.*/
	for (size_t i = 0; i < directionalLights.size(); ++i)
	{
		gameObjectManager->renderGameObjectsWithShadows(ssp);
	}
}

void ShadowObject::sortLights(std::vector<std::shared_ptr<LightObject>> lights)
{
	/*Sort each light into an array and gereate shadow maps for each directional light.*/
	for (auto light : lights)
	{
		switch (light->getLightType())
		{
		case LT_DIRECTIONAL:
			directionalLights.emplace_back(light);
			directionalShadowMaps.emplace_back(std::make_unique<ShadowMap>(getGameVars()->getRenderer()->getDevice(), ShadowMapSize, ShadowMapSize));
			break;

		case LT_POINT:
			pointLights.emplace_back(light);
			break;

		case LT_SPOT:
			spotLights.emplace_back(light);
			break;

		case LT_NONE:
		default:
			assert(false);
			//LIGHT TYPE NOT IMPLEMENTED!
			break;
		}
	}
}
