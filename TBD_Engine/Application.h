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

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	//Request browser
	void OpenLink(const char* link);
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

};