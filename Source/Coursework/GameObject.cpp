#include "GameObject.h"



GameObject::GameObject(GAME_OBJECT_TYPE gameObjectType)
	: gameVars(nullptr)
	, gameObjectType(gameObjectType)
	, isInitialised(false)
{
}

GameObject::GameObject(GameVariables * gameVars)
	: gameVars(gameVars)
{
}


GameObject::~GameObject()
{
}

void GameObject::init()
{
}

void GameObject::render()
{
}

void GameObject::renderDepth(Light* light, bool isDynamic=false)
{
}

void GameObject::renderShadows(ShadowShaderParams& ssp)
{
}

void GameObject::gui()
{
}

void GameObject::update()
{
}
