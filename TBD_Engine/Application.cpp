#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);

	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleEngineUI(this);
	mesh_loader = new MeshLoader(this);
	tex_loader = new TextureLoader(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(mesh_loader);
	AddModule(tex_loader);

	// Scenes
	AddModule(scene_intro);

	//UI
	AddModule(gui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete *item;
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::const_iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LogInConsole("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	
	frame_time.Start();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	new_sec_FrameCount++;

	dt = (float)frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000) {

		last_sec_frame_time.Start();
		prev_sec_FrameCount = new_sec_FrameCount;
		new_sec_FrameCount = 0;
	}

	Uint32 last_frame_ms = frame_time.Read();
	Uint32 frames_on_last_update = prev_sec_FrameCount;

	// FPS LOG
	fps_log.push_back(frames_on_last_update);
	if (fps_log.size() > 100)
	{
		fps_log.erase(fps_log.begin());
	}

	// MS LOG
	ms_log.push_back(last_frame_ms);
	if (ms_log.size() > 100)
	{
		ms_log.erase(ms_log.begin());
	}

}

void Application::OpenLink(const char* link)
{
	ShellExecuteA(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
}


void Application::LogInConsole(const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", tmp_string);
	OutputDebugString(tmp_string2);

	//pass the string to the vector of strings to later print them in the console 
	const char* string = tmp_string2;
	Logs_Console.push_back(string);

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::const_iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
