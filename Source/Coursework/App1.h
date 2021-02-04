// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h" //Changed from path because the compiler was too dumb to recognise that DXF.h 
#include "Scene.h"

class App1 : public BaseApplication
{
	std::unique_ptr<Scene> scene;
	std::unique_ptr<GameVariables> gameVariables;
public:
	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

private:

};

#endif