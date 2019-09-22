#ifndef __MODULEENGINEUI_H__
#define __MODULEENGINEUI_H__

#include "Module.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

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
	bool show_another_window = true;

	ImGuiIO* io = nullptr;

};

#endif
