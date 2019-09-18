#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleEngineUI.h"

ModuleEngineUI::ModuleEngineUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEngineUI::~ModuleEngineUI()
{}

// Load assets
bool ModuleEngineUI::Init()
{
	LOG("Loading UI");
	bool ret = true;
	

	return ret;
}

// Load assets
bool ModuleEngineUI::CleanUp()
{
	LOG("Unloading UI module");


	return true;
}

// Update
update_status ModuleEngineUI::Update(float dt)
{

	return UPDATE_CONTINUE;
}
