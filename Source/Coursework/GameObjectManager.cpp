#include "GameObjectManager.h"



GameObjectManager::GameObjectManager(GameVariables* gameVars)
	: isIteratingThroughGameObjs(false)
	, gameVars(gameVars)
{
}


GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::updateGameObjects()
{
	isIteratingThroughGameObjs = true;
	for (auto gameObject : gameObjects)
	{
		assert(gameObject->isInitialised); // Check if it has been initialised.
		gameObject->update();
	}
	isIteratingThroughGameObjs = false;

	//add remaining objects that were added during iteration
	addRemainingGameObjects();
}

void GameObjectManager::renderGameObjects()
{
	isIteratingThroughGameObjs = true;

	for (auto gameObject : gameObjects)
	{
		assert(gameObject->isInitialised); // Check if it has been initialised.
		gameObject->render();
	}

	isIteratingThroughGameObjs = false;
}

void GameObjectManager::renderDepthOfGameObjects(Light* light, bool isDynamic=true)
{
	isIteratingThroughGameObjs = true;

	for (auto gameObject : gameObjects)
	{
		assert(gameObject->isInitialised);
		gameObject->renderDepth(light, isDynamic);
	}

	isIteratingThroughGameObjs = false;
}

void GameObjectManager::renderGameObjectsWithShadows(ShadowShaderParams& ssp)
{
	isIteratingThroughGameObjs = true;

	for (auto gameObject : gameObjects)
	{
		assert(gameObject->isInitialised); // Check if it has been initialised.
		gameObject->renderShadows(ssp);
	}

	isIteratingThroughGameObjs = false;
}

void GameObjectManager::guiGameObjects()
{
	isIteratingThroughGameObjs = true;

	for (auto gameObject : gameObjects)
	{
		assert(gameObject->isInitialised); // Check if it has been initialised.
		gameObject->gui();
	}

	isIteratingThroughGameObjs = false;
}

void GameObjectManager::addGameObject(std::shared_ptr<GameObject> gameObj)
{
	if (isIteratingThroughGameObjs)
		gameObjectsToAdd.emplace(gameObj); // Add it later as not to break the iterator.
	else
		emplaceGameObject(gameObj); // Add it now because there is no iterator to break.
}

void GameObjectManager::emplaceGameObject(std::shared_ptr<GameObject> gameObj)
{
	assert(!isIteratingThroughGameObjs); // Make sure we're not adding a new object when we're iterating through the vector.
	gameObj->gameObjectManager = this;
	gameObj->gameVars = gameVars;
	assert(gameObj->isInitialised == false); // Make sure that it hasn't been initialsed before.
	gameObj->init();
	gameObj->isInitialised = true;
	gameObjects.push_back(gameObj);
}

void GameObjectManager::addRemainingGameObjects()
{
	assert(!isIteratingThroughGameObjs); // Make sure that we're not adding all the backlogged objects while iterating through the vector.

	while (!gameObjectsToAdd.empty())
	{
		emplaceGameObject(gameObjectsToAdd.front());
		gameObjectsToAdd.pop();
	}
}
