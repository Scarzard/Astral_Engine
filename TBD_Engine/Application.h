#pragma once

#include <list>

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEngineUI.h"
#include "ModuleEditor.h"

class Application
{
public:
	ModuleWindow*			window;
	ModuleInput*			input;
	ModuleSceneIntro*		scene_intro;
	ModuleRenderer3D*		renderer3D;
	ModuleCamera3D*			camera;
	ModuleEngineUI*			gui;
	ModuleEditor*			editor;

private:

	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	//Request browser
	void OpenLink(const char* link);

	void LogInConsole(const char* format, ...);
	std::vector<std::string>	Logs_Console;


	//Framerate logs
	std::vector<float>	ms_log;
	std::vector<float>	fps_log;
	int	MaxFPS = -1;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	//Framerate
	uint	frame_count = 0;
	Timer	time_since_start;
	Timer	frame_time;
	Timer	last_sec_frame_time;

	Uint32	new_sec_FrameCount = 0;
	Uint32	prev_sec_FrameCount = 0;
	float	dt = 0.0f;
	


};