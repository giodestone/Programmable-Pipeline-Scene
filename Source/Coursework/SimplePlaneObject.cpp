#include "SimplePlaneObject.h"



SimplePlaneObject::SimplePlaneObject(): textureAlias(nullptr), texturePath(nullptr), yPosGUI()
{
}


SimplePlaneObject::~SimplePlaneObject()
{
}

void SimplePlaneObject::init()
{
	planeMesh = std::make_unique<PlaneMesh>(getGameVars()->getRenderer()->getDevice(), getGameVars()->getRenderer()->getDeviceContext());
	getGameVars()->getTextureManager()->loadTexture(textureAlias, texturePath);

	yPosGUI = transformRef().getPosition().y;
}

void SimplePlaneObject::renderDepth(Light* light, bool isDynamic = false)
{
	light->generateViewMatrix();
	if (isDynamic)
	{
		light->generateOrthoMatrix(getGameVars()->getScreenDimensions().x, getGameVars()->getScreenDimensions().y, getGameVars()->getScreenNear(), getGameVars()->getScreenDepth());
	}

	//render with depth shader
	planeMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());
	getGameVars()->getShadowDepthShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		XMMatrixMultiply(getGameVars()->getRenderer()->getWorldMatrix(), transformRef().getTransformationMatrix()),
		light->getViewMatrix(),
		light->getOrthoMatrix(),
		ShadowDepthShaderParams()
	);

	getGameVars()->getShadowDepthShader()->render(getGameVars()->getRenderer()->getDeviceContext(), planeMesh->getIndexCount());
}

void SimplePlaneObject::renderShadows(ShadowShaderParams& ssp)
{
	//render with shadow shader
	planeMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());

	ssp.objectTexture = getGameVars()->getTextureManager()->getTexture(textureAlias);

	getGameVars()->getShadowShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		XMMatrixMultiply(getGameVars()->getRenderer()->getWorldMatrix(), transformRef().getTransformationMatrix()),
		getGameVars()->getMainCamera()->getViewMatrix(),
		getGameVars()->getRenderer()->getProjectionMatrix(),
		ssp
	);

	getGameVars()->getShadowShader()->render(getGameVars()->getRenderer()->getDeviceContext(), planeMesh->getIndexCount());
}

void SimplePlaneObject::gui()
{
	if (ImGui::CollapsingHeader("Plane"))
	{
		ImGui::DragFloat("Y Position", &yPosGUI, 0.01f);
	}
}

void SimplePlaneObject::update()
{
	transformRef().setPosition(transformRef().getPosition().x, yPosGUI, transformRef().getPosition().z);
}
