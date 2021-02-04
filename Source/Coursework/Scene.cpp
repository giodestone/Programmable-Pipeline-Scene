#include "Scene.h"



Scene::Scene(GameVariables* gameVariables)
	: gameObjectManager(gameVariables)
	, gameVariables(gameVariables)
{
	// Debug bunny sphere!
	//auto sphere = std::make_shared<SimpleSphereObject>();
	//sphere->transformRef().setPosition(0.f, 2.f, 15.f);
	//gameObjectManager.addGameObject(sphere);

	gameVariables->getMainCamera()->setPosition(10.f, 6.f, 10.f);
	
	auto tesselatedPlane = std::make_shared<TessellatedPlaneObject>();
	gameObjectManager.addGameObject(tesselatedPlane);

	auto bluePlane = std::make_shared<SimplePlaneObject>();
	bluePlane->setTextureAlias(L"water texture");
	bluePlane->setTexturePath(L"res/waterTexture.png");
	bluePlane->transformRef().setPosition(0.f, 3.5f, 0.f);
	gameObjectManager.addGameObject(bluePlane);

	auto wall1 = std::make_shared<ModelObject>();
	wall1->transformRef().setPosition(24.85f, 3.5f, 49.f);
	wall1->setModelPath("res/wall.obj");
	wall1->setTexturePath(L"res/brick1.dds"); //dreaded wall texture.
	wall1->setTextureAlias(L"wall1 tex");
	wall1->setName("Wall 1");
	gameObjectManager.addGameObject(wall1);

	auto wall2 = std::make_shared<ModelObject>();
	wall2->transformRef().setPosition(21.79f, 3.5f, 52.1f);
	wall2->transformRef().setRotation(0.f, 3.14159f / 2.f, 0.f);
	wall2->setModelPath("res/wall.obj");
	wall2->setTexturePath(L"res/brick1.dds");
	wall2->setTextureAlias(L"wall2 tex");
	wall2->setName("Wall 2");
	gameObjectManager.addGameObject(wall2);

	auto teapot = std::make_shared<ModelObject>();
	teapot->transformRef().setPosition(67.2f, 5.8f, 26.f);
	teapot->transformRef().setScale(0.2f);
	teapot->setModelPath("res/teapot.obj");
	teapot->setTexturePath(L"res/wood.png");
	teapot->setTextureAlias(L"teapot texture");
	teapot->setName("Teapot");
	gameObjectManager.addGameObject(teapot);

	auto teapot2 = std::make_shared<ModelObject>();
	teapot2->transformRef().setPosition(83.2f, 5.2f, 80.1f);
	teapot2->transformRef().setRotation(0.f, 1.86f, 0.f);
	teapot2->transformRef().setScale(0.2f);
	teapot2->setModelPath("res/teapot.obj");
	teapot2->setTexturePath(L"res/checkerboard.png");
	teapot2->setTextureAlias(L"teapot2 texture");
	teapot2->setName("Teapot 2");
	gameObjectManager.addGameObject(teapot2);
	
	auto bloomObj = std::make_shared<BloomPostProcessObject>();
	bloomPostProcess = bloomObj;
	gameObjectManager.addGameObject(bloomObj);

	auto shadowObj = std::make_shared<ShadowObject>();
	shadowObject = shadowObj;
	gameObjectManager.addGameObject(shadowObj);

	auto directionalLight = std::make_shared<LightObject>();
	directionalLight->setLightType(LT_DIRECTIONAL);
	directionalLight->setName("Directional 1");
	directionalLight->getLight()->setDirection(0.0f, -0.7f, 0.7f);
	directionalLight->getLight()->setPosition(50.f, 30.f, 0.f);
	directionalLight->getLight()->setAmbientColour(0.07f, 0.07f, 0.07f, 1.f); //only the first light needs an ambient colour.
	directionalLight->getLight()->setDiffuseColour(0.6f, 0.6f, 0.6f, 1.f);
	directionalLight->getLight()->generateOrthoMatrix(110.f, 110.f, 0.1, 200.f);
	directionalLight->getLight()->generateViewMatrix();
	lights.push_back(directionalLight);
	gameObjectManager.addGameObject(directionalLight);

	auto directionalLight2 = std::make_shared<LightObject>();
	directionalLight2->setLightType(LT_DIRECTIONAL);
	directionalLight2->setName("Directional 2");
	directionalLight2->getLight()->setDirection(0.9701f,-0.24254f, 0.f);
	directionalLight2->getLight()->setPosition(-60.f, 30.f, 50.f);
	
	directionalLight2->getLight()->setDiffuseColour(0.3f, 0.3f, 0.3f, 1.f);
	directionalLight2->getLight()->generateOrthoMatrix(110.f, 110.f, 0.1, 200.f);
	directionalLight2->getLight()->generateViewMatrix();
	lights.push_back(directionalLight2);
	gameObjectManager.addGameObject(directionalLight2);

	auto pointLight1 = std::make_shared<LightObject>();
	pointLight1->setLightType(LT_POINT);
	pointLight1->setName("Point 1");
	pointLight1->getLight()->setPosition(25.f, 5.6f, 52.f);
	pointLight1->getLight()->setDiffuseColour(1.f, 0.f, 0.f, 1.f);
	lights.push_back(pointLight1);
	gameObjectManager.addGameObject(pointLight1);
	
	auto pointLight2 = std::make_shared<LightObject>();
	pointLight2->setLightType(LT_POINT);
	pointLight2->setName("Point 2");
	pointLight2->getLight()->setPosition(80.5f, 5.6f, 78.4f);
	pointLight2->getLight()->setDiffuseColour(1.f, 0.f, 1.f, 1.f);
	lights.push_back(pointLight2);
	gameObjectManager.addGameObject(pointLight2);

	auto spotLight1 = std::make_shared<LightObject>();
	spotLight1->setLightType(LT_SPOT);
	spotLight1->setName("Spot 1");
	spotLight1->getLight()->setPosition(23.5f, 5.8f, 51.87f);
	spotLight1->getLight()->setDiffuseColour(0.3f, 1.f, 0.3f, 1.f);
	spotLight1->getLight()->setDirection(0.f, -0.75f, -0.65f);
	lights.push_back(spotLight1);
	gameObjectManager.addGameObject(spotLight1);

	auto spotLight2 = std::make_shared<LightObject>();
	spotLight2->setLightType(LT_SPOT);
	spotLight2->setName("Spot 2");
	spotLight2->getLight()->setPosition(69.3f, 7.32f, 29.0f);
	spotLight2->getLight()->setDirection(-0.21f, -0.93f, -0.29f);
	spotLight2->getLight()->setDiffuseColour(0.f, 1.f, 1.f, 1.f);
	lights.push_back(spotLight2);
	gameObjectManager.addGameObject(spotLight2);
}


Scene::~Scene()
{
}

void Scene::render()
{
	// Generate the view matrix based on the camera's position.
	gameVariables->getMainCamera()->update();

	
	shadowObject.lock()->depthPassObjects(&gameObjectManager, lights);

	/*Render scene to texture unless the bloom effect is disabled.*/
	if (bloomPostProcess.lock()->getIsEffectEnabled())
	{
		bloomPostProcess.lock()->beginRenderToTexture();
		shadowObject.lock()->renderWithShadows(&gameObjectManager, lights);
		bloomPostProcess.lock()->endRenderToTexture();

		bloomPostProcess.lock()->getBrightestPartsPass();
		bloomPostProcess.lock()->horizontalBlurPass();
		bloomPostProcess.lock()->verticalBlurPass();
		bloomPostProcess.lock()->mergeBloomIntoScene();
	}
	else
	{
		shadowObject.lock()->renderWithShadows(&gameObjectManager, lights);
		//render all of the objects
		//gameObjectManager.renderGameObjects();
	}
}

void Scene::gui()
{
	ImGui::Text("Having more than one header expanded will make\nImGUI confused and edit more than one value!");
	gameObjectManager.guiGameObjects();
}

void Scene::update()
{
	gameObjectManager.updateGameObjects();
}
