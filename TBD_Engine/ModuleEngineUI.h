#ifndef _ENGINEUI_H_
#define _ENGINEUI_H_

#include "Globals.h"
#include "Module.h"

#include "ImGui/imgui.h"
#include "ImGui/glew.h"

#include "ImGui/examples/imgui_impl_sdl.h"
#include "ImGui/examples/imgui_impl_opengl3.h"


#include "glut/glut.h"


class ModuleEngineUI : public Module
{
public:
	ModuleEngineUI(Application* app, bool start_enabled = true);
	~ModuleEngineUI();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	ImGuiIO* io = nullptr;

	bool show_demo_window = true;
	bool show_another_window = false;

};
#endif // !_ENGINEUI_H_
