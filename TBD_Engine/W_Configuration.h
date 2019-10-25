#ifndef _W_CONFIGURATION_H_
#define _W_CONFIGURATION_H_

#include "Module.h"
#include "Application.h"
#include "Window.h"

class W_Configuration : public Window
{
public:
	struct Hardware_Info {
		//CPU
		int cache_size = 0;
		int cpu_count = 0;
		int ram = 0;
		//GPU
		//int gpu_vendor = 0;
		//int gpu_device = 0;
		char gpu_brand[250] = "";
		float vram_budget = 0.f;
		float vram_usage = 0.f;
		float vram_available = 0.f;
		float vram_reserved = 0.f;
	};
	W_Configuration();
	~W_Configuration();

	bool Start();
	bool Draw();

private:
	float brightness_slider = 1.0f;
	int width_slider = SCREEN_WIDTH;
	int height_slider = SCREEN_HEIGHT;

	Hardware_Info info;

};

#endif // !_W_CONFIGURATION_H_