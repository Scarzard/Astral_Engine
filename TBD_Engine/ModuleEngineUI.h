#ifndef _ENGINEUI_H_
#define _ENGINEUI_H_

#include "Globals.h"
#include "Module.h"

#include "ImGui/imgui.h"
#include "ImGui/examples/imgui_impl_sdl.h"
#include "ImGui/examples/imgui_impl_opengl2.h"
#include "glut/glut.h"


class ModuleEngineUI : public Module
{
public:
	ModuleEngineUI(Application* app, bool start_enabled = true);
	~ModuleEngineUI();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	ImGuiIO* input = nullptr;

};
#endif // !_ENGINEUI_H_