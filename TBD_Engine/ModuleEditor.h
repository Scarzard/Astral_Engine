#ifndef _MODULEEDITOR_H_
#define _MODULEEDITOR_H_

#include "Module.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

//MGL includes

#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

//PCG includes
#include "PCG/entropy.h"
#include "PCG/pcg_variants.h"

//Parson
#include "parson/parson.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool show_demo_window = false;
	bool show_mgl_window = false;
	bool test_rng_window = false;
	bool configuration_window = false;
	bool about_window = false;
	bool show_console = true;

	float brightness_slider = 1.0f;
	int width_slider = SCREEN_WIDTH;
	int height_slider = SCREEN_HEIGHT;

	
	

	//RNG variables
	int min_, max_, min, max;

	float rn1 = 0.0f;
	uint32_t rn2 = 0;
	uint32_t rn3 = 0;
	int interval_random = 0.0f;

	pcg32_random_t rng; //typedef struct 64-b
	pcg_state_setseq_64 rng_bounded; // struct 64-b
	pcg_state_setseq_32 rng_bounded2; // struct 32-b

	//MGL
	Sphere sphere_1;
	Sphere sphere_2;

	AABB aabb_1;
	AABB aabb_2;

	Triangle triangle_1;
	Triangle triangle_2;

	bool spheresIntersecting, trianglesIntersecting, AABBIntersecting;
private:
	int cache_size = 0;
	int cpu_count = 0;
	int ram = 0;

};


#endif // !_MODULEEDITOR_H_

