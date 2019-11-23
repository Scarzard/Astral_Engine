#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);

	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleEngineUI(this);
	time = new TimeManager(this);
	mesh_loader = new MeshLoader(this);
	tex_loader = new TextureLoader(this);
	file_system = new ModuleFileSystem(true, ASSETS_FOLDER);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(time);
	AddModule(mesh_loader);
	AddModule(tex_loader);
	AddModule(file_system);
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
	dt = (float)frame_time.ReadSec();
	frame_time.Start();

	switch (state)
	{

	case ENGINE_STATE::IN_EDITOR:
		using_dt = dt;
		break;
	case ENGINE_STATE::PLAY:
	case ENGINE_STATE::PAUSE:
		using_dt = time->GetGameDT();
	}

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
	sprintf_s(tmp_string2, 4096, "\n%s", tmp_string);
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

bool Application::Play()
{
	switch (state)
	{
	case ENGINE_STATE::IN_EDITOR:
		if (camera->GetActiveCamera() != nullptr) 
		{
			//Change camera view
			camera->active_camera = camera->obj_camera->GetComponentCamera();
			camera->active_camera->has_transformed = true;
			ForceEngineState(ENGINE_STATE::PLAY);

			//We need to save the scene 

			return true;
		}
		else
			LogInConsole("There's no active camera!");

		break;
	}

	return false;
}

void Application::Pause()
{
	switch (state)
	{
	case ENGINE_STATE::PLAY:
		ForceEngineState(ENGINE_STATE::PAUSE);
		break;

	case ENGINE_STATE::PAUSE:
		ForceEngineState(ENGINE_STATE::PLAY);
		break;
	}
}

void Application::Stop()
{
	switch (state) {
	case ENGINE_STATE::PLAY:
	case ENGINE_STATE::PAUSE:
		//Change camera view
		camera->active_camera = camera->main_camera;
		camera->main_camera->has_transformed = true;

		ForceEngineState(ENGINE_STATE::IN_EDITOR);

		//Load the scene we saved just before hitting play

		time->ResetGameTimer();

		break;
	}
}

void Application::ForceEngineState(ENGINE_STATE state)
{
	this->state = state;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

ENGINE_STATE Application::GetState()
{
	return state;
}

const std::string Application::GetNameFromPath(std::string path)
{
	std::string name = path;

	uint num = name.find_last_of("/\\");
	name = name.substr(num + 1, name.size());

	uint dot = name.find_last_of(".");
	name = name.substr(0, dot);

	return name;
}

const std::string Application::GetDirectoryFromPath(std::string path)
{
	std::string directory;
	size_t found = path.find_last_of("/\\");
	directory = path.substr(0, found);

	return directory;
}

const std::string Application::GetFileExtension(const std::string FileName)
{
	if (FileName.find_last_of(".") != std::string::npos)
		return (FileName.substr(FileName.find_last_of(".") + 1));
	else
		return "";
}

void Application::eraseSubStr(std::string & mainStr, const std::string & toErase)
{
	// Search for the substring in string
	size_t pos = mainStr.find(toErase);

	if (pos != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

float Application::GetDT()
{
	return dt;
}
