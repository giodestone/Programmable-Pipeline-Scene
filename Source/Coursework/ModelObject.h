#pragma once
#include "GameObject.h"

#include "Model.h"
#include <array>

/// <summary>
/// Wrapper around the model class
/// </summary>
class ModelObject :
	public GameObject
{
	char* modelPath;
	wchar_t* textureAlias;
	wchar_t* texturePath;

	std::unique_ptr<Model> model;

	std::array<float, 3> positionsGUI;
	std::array<float, 3> rotationGUI;
	std::string guiName;
	
public:
	ModelObject();
	~ModelObject();
protected:
	/// <summary>
	/// Load and setup the model and texture.
	/// </summary>
	void init() override;

	/// <summary>
	/// Render the depth pass for this object using the regular shadow shader.
	/// </summary>
	/// <param name="light"></param>
	/// <param name="isDynamic"></param>
	void renderDepth(Light* light, bool isDynamic) override;

	/// <summary>
	/// Render the object with shadows for this object using the regular shadow shader.
	/// </summary>
	/// <param name="ssp"></param>
	void renderShadows(ShadowShaderParams& ssp) override;

	/// <summary>
	/// Draw stuff for GUI.
	/// </summary>
	void gui() override;

	/// <summary>
	/// Update transform from gui arrays.
	/// </summary>
	void update() override;
	
public:
	/// <summary>
	/// Set model of the path.
	/// </summary>
	void setModelPath(char* modelPath) { this->modelPath = modelPath; }

	/// <summary>
	/// Set the alias of the texture that will be loaded.
	/// </summary>
	void setTextureAlias(wchar_t* textureAlias) { this->textureAlias = textureAlias; }

	/// <summary>
	/// Set the path of the texture that will be loaded.
	/// </summary>
	void setTexturePath(wchar_t* texturePath) { this->texturePath = texturePath; }

	/// <summary>
	/// Set the name that will be displayed on the GUI.
	/// </summary>
	/// <param name="name"></param>
	void setName(std::string name) { this->guiName = name; }
};

