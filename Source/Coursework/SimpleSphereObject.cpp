#include "SimpleSphereObject.h"



SimpleSphereObject::SimpleSphereObject()
{
}


SimpleSphereObject::~SimpleSphereObject()
{
}

void SimpleSphereObject::init()
{
	sphereMesh = std::make_unique<SphereMesh>(getGameVars()->getRenderer()->getDevice(), getGameVars()->getRenderer()->getDeviceContext());

	getGameVars()->getTextureManager()->loadTexture(L"bunny", L"res/bunny.png");

	position[0] = transformRef().getPosition().x; // set position to be one set intially.
	position[1] = transformRef().getPosition().y;
	position[2] = transformRef().getPosition().z;
}

void SimpleSphereObject::update()
{
	transformRef().setPosition(position[0], position[1], position[2]);
}

void SimpleSphereObject::renderDepth(Light* light, bool isDynamic=false)
{
	light->generateViewMatrix();
	if (isDynamic)
	{
		light->generateOrthoMatrix(getGameVars()->getScreenDimensions().x, getGameVars()->getScreenDimensions().y, getGameVars()->getScreenNear(), getGameVars()->getScreenDepth());
	}
	/*Reder the mesh to the shadow map.*/
	sphereMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());

	getGameVars()->getShadowDepthShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		XMMatrixMultiply(getGameVars()->getRenderer()->getWorldMatrix(), transformRef().getTransformationMatrix()),
		light->getViewMatrix(),
		light->getOrthoMatrix(),
		ShadowDepthShaderParams()
	);

	getGameVars()->getShadowDepthShader()->render(getGameVars()->getRenderer()->getDeviceContext(), sphereMesh->getIndexCount());
}

void SimpleSphereObject::renderShadows(ShadowShaderParams& ssp)
{
	sphereMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());

	ssp.objectTexture = getGameVars()->getTextureManager()->getTexture(L"bunny");

	getGameVars()->getShadowShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		XMMatrixMultiply(getGameVars()->getRenderer()->getWorldMatrix(), transformRef().getTransformationMatrix()),
		getGameVars()->getMainCamera()->getViewMatrix(),
		getGameVars()->getRenderer()->getProjectionMatrix(),
		ssp
	);

	getGameVars()->getShadowShader()->render(getGameVars()->getRenderer()->getDeviceContext(), sphereMesh->getIndexCount());
}

void SimpleSphereObject::gui()
{
	if (ImGui::CollapsingHeader("Ball"))
	{
		ImGui::DragFloat3("Position", position.data(), 0.01f);
	}
}