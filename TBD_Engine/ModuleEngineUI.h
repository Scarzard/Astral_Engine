#ifndef __MODULEENGINEUI_H__
#define __MODULEENGINEUI_H__

#include "Module.h"
#include "ImGui/imgui.h"



class ModuleEngineUI : public Module
{
public:

	ModuleEngineUI(Application* app, bool start_enabled = true);
	~ModuleEngineUI();

	bool Init();
	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	ImGuiIO* io = nullptr;
	bool p_open = true;
	
};

#endif
