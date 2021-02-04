// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	/* Initalise scene variables. */
	gameVariables = std::make_unique<GameVariables>();
	gameVariables->input = input;
	gameVariables->mainCamera = camera;
	gameVariables->renderer = renderer;
	gameVariables->textureManager = textureMgr;
	gameVariables->timer = timer;
	gameVariables->hwnd = wnd;

	gameVariables->screenNear = SCREEN_NEAR;
	gameVariables->screenDepth = SCREEN_DEPTH;

	gameVariables->screenDimensions.x = (float)screenWidth;
	gameVariables->screenDimensions.y = (float)screenHeight;

	gameVariables->initShaders();

	scene = std::make_unique<Scene>(gameVariables.get());

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	// [pc, memory leak -->] 💻🧨- - - 🔫 [<-- smart pointer]
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	//Update things
	scene->update();

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	scene->render();
	
	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	scene->gui();

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

