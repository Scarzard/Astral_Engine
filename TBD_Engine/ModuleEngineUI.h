#ifndef __MODULEENGINEUI_H__
#define __MODULEENGINEUI_H__

#include "Module.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

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

	bool show_demo_window = true;
	bool show_another_window = false;
	bool test_rng_window = true;

	ImGuiIO* io = nullptr;

	//RNG variables
	float rn1 = 0.0f;
	uint32_t rn2 = 0;
	uint32_t rn3 = 0;

	pcg32_random_t rng; //typedef struct 64-b
	pcg32_random_t rng1; //typedef struct 64-b
	pcg_state_setseq_64 rng_bounded; // struct 64-b
	pcg_state_setseq_64 rng_bounded2;
};

#endif
