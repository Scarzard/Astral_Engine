#ifndef __MODULEENGINEUI_H__
#define __MODULEENGINEUI_H__

#include "Module.h"
#include "Window.h"
#include "ImGui/imgui.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "W_Game.h"
#include "W_Hierarchy.h"
#include "W_Console.h"
#include "W_Inspector.h"
#include "W_Configuration.h"

class ModuleEngineUI : public Module
{
public:

	ModuleEngineUI(bool start_enabled = true);
	~ModuleEngineUI();

	bool Init();
	bool Start();
	bool Draw();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void CreateMainMenuToolbar();

public:
	bool is_game_focused = false;

	bool console = true;
	bool config = true;
	bool hierarchy = true;
	bool game = true;
	bool inspector = true;

	W_Game* game_window = nullptr;
	W_Inspector* ins_window = nullptr;
	W_Console* console_window = nullptr;
	W_Hierarchy* h_window = nullptr;
	W_Configuration* conf_window = nullptr;
	
	bool save_scene_pop = false;
private:

	//bool show_demo_window = true;
	ImGuiIO* io = nullptr;
	bool p_open = true;
	bool demo_cpp = false;
	bool about_window = false;
	//Store the windows here and iterate them in Start, Draw, Pre/Post Update 
	std::list<Window*> windows;

};

#endif
