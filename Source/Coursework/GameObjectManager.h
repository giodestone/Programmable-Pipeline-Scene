#pragma once
#include <memory>
#include <queue>
#include <vector>

#include "GameObject.h"
#include "GameVariables.h"

class GameObjectManager
{
	std::vector<std::shared_ptr<GameObject>> gameObjects;

	bool isIteratingThroughGameObjs; // So the iterator is ok if an new object gets added during runtime.
	std::queue<std::shared_ptr<GameObject>> gameObjectsToAdd;

	GameVariables* gameVars;
public:
	GameObjectManager(GameVariables* gameVars);
	~GameObjectManager();

	/// <summary>
	/// Call update on all of the game objects.
	/// </summary>
	void updateGameObjects();

	/// <summary>
	/// Call render the game objects.
	/// </summary>
	void renderGameObjects();

	/// <summary>
	/// Call render depth on game objects.
	/// </summary>
	void renderDepthOfGameObjects(Light* light, bool isDynamic);

	/// <summary>
	/// Call render with shadows on game objects.
	/// </summary>
	/// <param name="ssp"></param>
	void renderGameObjectsWithShadows(ShadowShaderParams& ssp);

	/// <summary>
	/// Add game object GUI elements.
	/// </summary>
	void guiGameObjects();

	/// <summary>
	/// Add a game object to the manager.
	/// </summary>
	/// <remarks>
	/// Doesn't add the object when iterating through instantly.
	/// </remarks>
	/// <param name="gameObj"></param>
	void addGameObject(std::shared_ptr<GameObject> gameObj);

private:
	/// <summary>
	/// Add a game object to the main game object array list and populate the variables that this class should.
	/// </summary>
	/// <param name="gameObj"></param>
	void emplaceGameObject(std::shared_ptr<GameObject> gameObj);

	/// <summary>
	/// Add the backlogged objects that were added in when iterating.
	/// </summary>
	void addRemainingGameObjects();
};

