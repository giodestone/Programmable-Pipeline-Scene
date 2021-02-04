#include "LightObject.h"



LightObject::LightObject()
	: lightType(LT_NONE), light(std::make_unique<Light>()), positionGUI(), directionGUI()
{
}


LightObject::~LightObject()
{
}

void LightObject::init()
{
	positionGUI[0] = light->getPosition().x;
	positionGUI[1] = light->getPosition().y;
	positionGUI[2] = light->getPosition().z;

	directionGUI[0] = light->getDirection().x;
	directionGUI[1] = light->getDirection().y;
	directionGUI[2] = light->getDirection().z;

	diffuseGUI[0] = light->getDiffuseColour().x;
	diffuseGUI[1] = light->getDiffuseColour().y;
	diffuseGUI[2] = light->getDiffuseColour().z;
	
	debugSphere = std::make_unique<SphereMesh>(getGameVars()->getRenderer()->getDevice(), getGameVars()->getRenderer()->getDeviceContext());
	transformRef().setScale(0.25f);
}

void LightObject::gui()
{
	/*Compile name.*/
	std::string nameStr;
	nameStr += "Light: ";
	nameStr += name;

	if (ImGui::CollapsingHeader(nameStr.data()))
	{
		ImGui::Checkbox("Draw Debug Sphere", &isDebugSphereEnabled);
		ImGui::ColorEdit3("Diffuse", diffuseGUI.data());
		ImGui::DragFloat3("Position", positionGUI.data(), 0.01f);

		/*Don't include direction if its a point light.*/
		if (lightType != LIGHT_TYPE::LT_POINT)
		{
			ImGui::DragFloat3("Direction", directionGUI.data(), 0.001f, -1.f, 1.f);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("WARNING: SETTING ALL VALUES TO ZERO WILL CAUSE ASSERTION!.");
			}	
		}
	}
}

void LightObject::update()
{
	light->setPosition(positionGUI[0], positionGUI[1], positionGUI[2]);
	light->setDirection(directionGUI[0], directionGUI[1], directionGUI[2]);
	light->setDiffuseColour(diffuseGUI[0], diffuseGUI[1], diffuseGUI[2], light->getDiffuseColour().w);
	transformRef().setPosition(positionGUI[0], positionGUI[1], positionGUI[2]);
}

void LightObject::renderShadows(ShadowShaderParams& ssp)
{
	/*Doesn't render with shadows.*/
	if (isDebugSphereEnabled)
	{
		debugSphere->sendData(getGameVars()->getRenderer()->getDeviceContext());

		auto worldTransform = XMMatrixMultiply(getGameVars()->getRenderer()->getWorldMatrix(), transformRef().getTransformationMatrix());

		SimpleShaderParams simpleShaderParams;
		simpleShaderParams.texture = getGameVars()->getTextureManager()->getTexture(L"default");
		
		getGameVars()->getSimpleShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(), worldTransform, getGameVars()->getMainCamera()->getViewMatrix(), getGameVars()->getRenderer()->getProjectionMatrix(), simpleShaderParams);	
		getGameVars()->getSimpleShader()->render(getGameVars()->getRenderer()->getDeviceContext(), debugSphere->getIndexCount());
	}
}
