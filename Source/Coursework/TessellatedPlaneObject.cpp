#include "TessellatedPlaneObject.h"



TessellatedPlaneObject::TessellatedPlaneObject()
	: GameObject(GAME_OBJECT_TYPE::GOT_TESSELATED_PLANE)
	, tessellationModifier(1.f)
	, amplitude(12.f)
{
}


TessellatedPlaneObject::~TessellatedPlaneObject()
{
}

void TessellatedPlaneObject::init()
{
	tessellationPlaneMesh = std::make_unique<TessellationPlaneMesh>(getGameVars()->getRenderer()->getDevice(), 100);
	getGameVars()->getTextureManager()->loadTexture(L"heightmap", L"res/heightMap4k.png");
	getGameVars()->getTextureManager()->loadTexture(L"grass texture", L"res/grassTexture.jpg");
	getGameVars()->getTextureManager()->loadTexture(L"grass texture", L"res/wood.png");
}

void TessellatedPlaneObject::renderDepth(Light * light, bool isDynamic)
{
	light->generateViewMatrix();
	if (isDynamic)
	{
		light->generateOrthoMatrix(getGameVars()->getScreenDimensions().x, getGameVars()->getScreenDimensions().y, getGameVars()->getScreenNear(), getGameVars()->getScreenDepth());
	}

	/*Send data to shader and render.*/
	tessellationPlaneMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());
	getGameVars()->getShadowTessellationDepthShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(), transformRef().getTransformationMatrix(), light->getViewMatrix(), light->getOrthoMatrix(), getPlaneTessellationParams());
	getGameVars()->getShadowTessellationDepthShader()->render(getGameVars()->getRenderer()->getDeviceContext(), tessellationPlaneMesh->getIndexCount());
}

void TessellatedPlaneObject::renderShadows(ShadowShaderParams& ssp)
{
	ssp.objectTexture = getGameVars()->getTextureManager()->getTexture(L"grass texture");

	tessellationPlaneMesh->sendData(getGameVars()->getRenderer()->getDeviceContext());

	getGameVars()->getShadowTessellationShader()->setShaderParams(getGameVars()->getRenderer()->getDeviceContext(),
		XMMatrixMultiply(getGameVars()->getRenderer()->getWorldMatrix(), transformRef().getTransformationMatrix()),
		getGameVars()->getMainCamera()->getViewMatrix(),
		getGameVars()->getRenderer()->getProjectionMatrix(),
		getPlaneTessellationParams(),
		ssp
	);

	getGameVars()->getShadowTessellationShader()->render(getGameVars()->getRenderer()->getDeviceContext(), tessellationPlaneMesh->getIndexCount());
}

void TessellatedPlaneObject::gui()
{
	if (ImGui::CollapsingHeader("Tessellated Plane"))
	{
		ImGui::DragFloat("AmplitudeModifier", &amplitude, 0.01f, 1.f, 1000.f);
		ImGui::DragFloat("Tesselation at Near", &tessellationAtNear, 0.01f, 1.f, 64.f);
		ImGui::DragFloat("Tesselation at Far", &tessellationAtFar, 0.01f, 1.f, 64.f);
		ImGui::DragFloat("Near Distance", &nearDistance, 0.01f, nearDistance - 0.01f, 1000.f);
		ImGui::DragFloat("Far Distance", &farDistance, 0.01f, 0.f, farDistance + 0.01f);
	}
}

PlaneTessellationParams TessellatedPlaneObject::getPlaneTessellationParams() const
{
	PlaneTessellationParams ptp;
	ptp.cameraPosition = getGameVars()->getMainCamera()->getPosition();
	ptp.heightmap = getGameVars()->getTextureManager()->getTexture(L"heightmap");
	ptp.texture = getGameVars()->getTextureManager()->getTexture(L"grass texture");
	ptp.nearLOD = tessellationAtNear;
	ptp.farLOD = tessellationAtFar;
	ptp.farDistance = farDistance;
	ptp.nearDistance = nearDistance;
	ptp.amplitude = amplitude;
	ptp.heightmapDimensions = XMFLOAT2(4096.f, 4096.f);

	return ptp;
}
