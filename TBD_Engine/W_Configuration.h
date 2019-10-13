#ifndef _W_CONFIGURATION_H_
#define _W_CONFIGURATION_H_

#include "Module.h"
#include "Application.h"
#include "Window.h"

class W_Configuration : public Window
{
public:
	W_Configuration();
	~W_Configuration();

	bool Start();
	bool Draw();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	int cache_size = 0;
	int cpu_count = 0;
	int ram = 0;

	float brightness_slider = 1.0f;
	int width_slider = SCREEN_WIDTH;
	int height_slider = SCREEN_HEIGHT;
};

#endif // !_W_CONFIGURATION_H_