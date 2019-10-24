#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

public:
	//Setters
	void SetTitle(const char* title);

	void SetBrightness(float brightness);
	void SetWidth(uint width);
	void SetHeigth(uint height);
	void SetWindowSize(int width, int height);
	void SetFullscreen(bool full);
	void SetBorderless(bool border);
	void SetFullScreenDesktop(bool desktop);
	void SetResizable(bool resize);

	//Getters
	float GetBrightness() const;
	uint GetWidth() const;
	uint GetHeight() const;
	void GetWindowSize(int &width, int &height);
	bool GetFullscreenWindow() const;
	bool GetFullDesktopWindow() const;
	bool GetBorderlessWindow() const;


public:
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__