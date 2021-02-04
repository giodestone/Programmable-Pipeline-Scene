#pragma once
#include "GameObjectManager.h"
#include "SimpleSphereObject.h"
#include "TessellatedPlaneObject.h"
#include "BloomPostProcessObject.h"
#include "ShadowObject.h"
#include "SimplePlaneObject.h"
#include "LightObject.h"
#include "ModelObject.h"

class Scene
{
	GameObjectManager gameObjectManager;
	std::vector<std::shared_ptr<LightObject>> lights;
	std::weak_ptr<BloomPostProcessObject> bloomPostProcess;
	std::weak_ptr<ShadowObject> shadowObject;

	GameVariables* gameVariables;
public:
	Scene(GameVariables* gameVariables);
	~Scene();

	void render();
	void gui();
	void update();
};

