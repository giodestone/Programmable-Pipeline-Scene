#pragma once
#include <cassert>
#include "Transform.h"
#include "GameVariables.h"
#include "ENUM_GameObjectType.h"

class GameObjectManager; //forward declaration

class GameObject
{
protected:
	Transform transform;

	GAME_OBJECT_TYPE gameObjectType;

public:
	GameObject(GAME_OBJECT_TYPE gameObjectType=GAME_OBJECT_TYPE::GOT_NONE);
	GameObject(GameVariables* gameVars);
	virtual ~GameObject();

	/// <summary>
	/// Get a copy of the transform.
	/// </summary>
	/// <returns>Copy of Transform.</returns>
	Transform getTrasform() const noexcept
	{
		return transform;
	}

	void setTransform(const Transform transform) noexcept
	{
		this->transform = transform;
	}

	/// <summary>
	/// Get a reference to the transform.
	/// </summary>
	/// <returns>Transform.</returns>
	Transform& transformRef()
	{
		return transform;
	}

	GAME_OBJECT_TYPE getGameObjectType() const noexcept
	{
		return gameObjectType;
	}

protected:
	/// <summary>
	/// For initialising variables.
	/// </summary>
	/// <remarks>
	/// Called by GameObjectManager when made 'active'. Do not call unless you want it to be double initilaised.
	/// </remarks>
	virtual void init();
	
	/// <summary>
	/// For rending without shadows (used for debugging only).
	/// </summary>
	virtual void render();

	/// <summary>
	/// For rendering the depth of the object.
	/// </summary>
	/// <param name="light">Which light to render the depth for.</param>
	/// <param name="isDynamic">Whether the light is dynamic and should have its matrices recalculated.</param>
	virtual void renderDepth(Light* light, bool isDynamic);

	/// <summary>
	/// For rendering the shadows of the object.
	/// </summary>
	/// <param name="ssp"></param>
	virtual void renderShadows(ShadowShaderParams& ssp);

	/// <summary>
	/// For adding GUI elements.
	/// </summary>
	virtual void gui();

	/// <summary>
	/// For GameObject logic.
	/// </summary>
	virtual void update();

private:
	/*Only GameObjectManager can access these.*/
	friend GameObjectManager;
	
	GameObjectManager* gameObjectManager; // For adding new objects inside of the game object.
	GameVariables* gameVars; // For accessing renderer, timer, hwnd, shaders etc.

	bool isInitialised;

	/// <summary>
	/// Set the game variables.
	/// </summary>
	/// <remarks>Only should be accessed by the GameObjectManager. Assert set to check if not null.</remarks>
	/// <param name="gameVars"></param>
	void setGameVars(GameVariables* gameVars)
	{
		assert(gameVars == nullptr);

		this->gameVars = gameVars;
	}

protected:
	/// <summary>
	/// Get the Game Variables.
	/// </summary>
	/// <remarks>Should be set by the GameObjectManager beforehand. Assert contained to check if null.</remarks>
	/// <returns></returns>
	GameVariables* getGameVars() const
	{
		assert(gameVars != nullptr);
		return gameVars;
	}

	/// <summary>
	/// Get the Game Object Manager.
	/// </summary>
	/// <remarks>Should be seet by the GameObjectManager beforehand. Assert contained to check if null.</remarks>
	/// <returns></returns>
	GameObjectManager* getGameObjectManager() const
	{
		assert(gameObjectManager != nullptr);
		return gameObjectManager;
	}
};

