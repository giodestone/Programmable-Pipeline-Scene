#include "ModelObject.h"



ModelObject::ModelObject(): modelPath(nullptr), textureAlias(nullptr), texturePath(nullptr)
{
}


ModelObject::~ModelObject()
{
}

void ModelObject::init()
{
	positionsGUI[0] = transformRef().getPosition().x;
	positionsGUI[1] = transformRef().getPosition().y;
	positionsGUI[2] = transformRef().getPosition().z;

	rotationGUI[0] = transformRef().getRotation().x;
	rotationGUI[1] = transformRef().getRotation().y;
	rotationGUI[2] = transformRef().getRotation().z;
	
	model = std::make_unique<Model>(getGameVars()->getRenderer()->getDevice(), getGameVars()->getRenderer()->getDeviceContext(), modelPath);

	getGameVars()->getTextureManager()->loadTexture(textureAlias, texturePath);
}

void ModelObject::renderDepth(Light* light, bool isDynamic)
{
	light->generateViewMatrix();
	if (isDynamic)
	{
		light->generateOrthoMatrix(getGameVars()->getScreenDimensions().x, getGameVars()->getScreenDimensions().y, getGameVars()->getScreenNear(), getGameVars()->getScreenDepth());
	}
	/*Render the mesh to the shadow map.*/
	model->sendData(getGameVars()->getRenderer()->getDeviceContext());

	getGameVars()->getShadowDepthShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		XMMatrixMultiply(getGameVars()->getRenderer()->getWorldMatrix(), transformRef().getTransformationMatrix()),
		light->getViewMatrix(),
		light->getOrthoMatrix(),
		ShadowDepthShaderParams()
	);

	getGameVars()->getShadowDepthShader()->render(getGameVars()->getRenderer()->getDeviceContext(), model->getIndexCount());
}

void ModelObject::renderShadows(ShadowShaderParams& ssp)
{
	model->sendData(getGameVars()->getRenderer()->getDeviceContext());

	ssp.objectTexture = getGameVars()->getTextureManager()->getTexture(textureAlias);

	getGameVars()->getShadowShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		XMMatrixMultiply(getGameVars()->getRenderer()->getWorldMatrix(), transformRef().getTransformationMatrix()),
		getGameVars()->getMainCamera()->getViewMatrix(),
		getGameVars()->getRenderer()->getProjectionMatrix(),
		ssp
	);

	getGameVars()->getShadowShader()->render(getGameVars()->getRenderer()->getDeviceContext(), model->getIndexCount());
}

void ModelObject::gui()
{
	std::string name;
	name += "Model ";
	name += guiName;

	if (ImGui::CollapsingHeader(name.data()))
	{
		ImGui::DragFloat3("Position", positionsGUI.data(), 0.01f);
		ImGui::DragFloat3("Rotation", rotationGUI.data(), 0.01f);
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Pitch, Yaw, Roll");
		}
	}
}

void ModelObject::update()
{
	transformRef().setPosition(positionsGUI[0], positionsGUI[1], positionsGUI[2]);
	transformRef().setRotation(rotationGUI[0], rotationGUI[1], rotationGUI[2]);
}
