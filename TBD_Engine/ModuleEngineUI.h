#ifndef _ENGINEUI_H_
#define _ENGINEUI_H_

#include "ImGui/imgui.h"

#include "ImGui/examples/imgui_impl_opengl3.h"
#include "ImGui/examples/imgui_impl_glfw.h"

class ModuleEngineUI : public Module
{
public:
	ModuleEngineUI(Application* app, bool start_enabled = true);
	~ModuleEngineUI();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

};
#endif // !_ENGINEUI_H_

