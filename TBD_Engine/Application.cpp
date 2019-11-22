#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEngineUI.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "ModuleFileSystem.h"

#include <fstream>
#include <iomanip>

#include "mmgr/mmgr.h"

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
	file_system = new ModuleFileSystem(true, ASSETS_FOLDER);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(mesh_loader);
	AddModule(tex_loader);
	AddModule(file_system);
	// Scenes
	AddModule(scene_intro);

	//UI
	AddModule(gui);

	// Renderer last!
	AddModule(renderer3D);

	SettingsPath = "Settings/config.json";
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

	//Load config
	LoadSettings();

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
	//Save config
	SaveSettings();

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


void Application::LoadSettings()
{
	json config;

	//If the adress of the settings file is null, create  an exception
	assert(SettingsPath != nullptr);

	//Create a stream and open the file
	std::ifstream stream;
	stream.open(SettingsPath);

	//Load configuration for all the modules
	std::list<Module*>::iterator it = list_modules.begin();

	config = json::parse(stream);

	//close the file
	stream.close();

	std::string name = config["Application"]["Name"];
	NameEngine = name;

	std::string org = config["Application"]["Organization"];
	Organization = org;

	std::string version = config["Application"]["Version"];
	VerisonEngine = version;

	while (it != list_modules.end())
	{
		(*it)->Load(config);
		it++;
	}
}

void Application::SaveSettings()
{
	//Create auxiliar file
	json config;
	config["Application"]["Name"] = NameEngine;
	config["Application"]["Version"] = VerisonEngine;
	config["Application"]["Organization"] = Organization;

	//Save configuration for all the modules
	std::list<Module*>::iterator it = list_modules.begin();

	while (it != list_modules.end())
	{
		(*it)->Save(config);
		it++;
	}

	//Create the stream and open the file
	std::ofstream stream;
	stream.open(SettingsPath);
	stream << std::setw(4) << config << std::endl;
	stream.close();
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