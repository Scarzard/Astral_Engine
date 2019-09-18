#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	ui = new ModuleEngineUI(this);
	//physics = new ModulePhysics3D(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(ui);
	//AddModule(physics);
	
	// Scenes
	AddModule(scene_intro);


	// Renderer last!
	AddModule(renderer3D);

	
}

Application::~Application()
{
	list<Module*>::reverse_iterator item = module_list.rbegin();

	while (item != module_list.rend())
	{
		delete (*item);
		item++;
	}

	module_list.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	list<Module*> ::iterator item = module_list.begin();

	while (item != module_list.end() && ret)
	{
		if ((*item)->enabled)
		{
			ret = (*item)->Init();
		}
		
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	while (item != module_list.end() && ret)
	{
		if ((*item)->enabled)
		{
			ret = (*item)->Start();
		}
		item++;
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	list<Module*> ::const_iterator item = module_list.begin();
	
	while(item != module_list.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->enabled)
		{
			ret = (*item)->PreUpdate(dt);
		}
		
		item++;
	}

	item = module_list.begin();

	while (item != module_list.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->enabled)
		{
			ret = (*item)->Update(dt);
		}

		item++;
	}

	item = module_list.begin();

	while(item != module_list.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->enabled)
		{
			ret = (*item)->PostUpdate(dt);
		}

		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	list<Module*>::reverse_iterator item = module_list.rbegin();

	while (item != module_list.rend() && ret == true)
	{
		if ((*item)->enabled)
		{
			ret = (*item)->CleanUp();
		}
		item++;
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	module_list.push_back(mod);
}