#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

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
	void SetSize(uint size, int& w, int& h);
	void SetFullscreen(bool full);
	void SetBorderless(bool border);
	void SetFullScreenDesktop(bool desktop);
	void SetResizable(bool resize);

	//Getters
	float GetBrightness() const;
	uint GetWidth() const;
	uint GetHeight() const;
	void GetWindowSize(int &width, int &height);
	uint GetSize() const;
	bool GetFullscreenWindow() const;
	bool GetFullDesktopWindow() const;
	bool GetBorderlessWindow() const;


public:
	int screen_w = SCREEN_WIDTH;
	int screen_h = SCREEN_HEIGHT;

	uint size = SCREEN_SIZE;

	int width = SCREEN_WIDTH * SCREEN_SIZE;
	int height = SCREEN_HEIGHT * SCREEN_SIZE;

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