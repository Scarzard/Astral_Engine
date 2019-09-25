#ifndef __MODULEENGINEUI_H__
#define __MODULEENGINEUI_H__

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

class ModuleEngineUI : public Module
{
public:

	ModuleEngineUI(Application* app, bool start_enabled = true);
	~ModuleEngineUI();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	//void Draw() const;

	bool main_window = true;
	bool show_demo_window = false;
	bool show_mgl_window = false;
	bool test_rng_window = false;

	ImGuiIO* io = nullptr;

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
	float x, y, z;
	bool isTouching;
};

#endif
