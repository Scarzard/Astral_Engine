#pragma once

#include <vector>

#include "Globals.h"
#include "Timer.h"
#include "Module.h"



class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEngineUI;
class MeshLoader;
class TextureLoader;
class ModuleFileSystem;



using json = nlohmann::json;

class Application
{
public:
	ModuleWindow*			window = nullptr;
	ModuleInput*			input = nullptr;
	ModuleSceneIntro*		scene_intro = nullptr;
	ModuleRenderer3D*		renderer3D = nullptr;
	ModuleCamera3D*			camera = nullptr;
	ModuleEngineUI*			gui = nullptr;
	MeshLoader*				mesh_loader = nullptr;
	TextureLoader*			tex_loader = nullptr;
	ModuleFileSystem*		file_system = nullptr;

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


	const std::string GetNameFromPath(std::string path);
	const std::string GetDirectoryFromPath(std::string path);
	const std::string GetFileExtension(const std::string FileName);
	void eraseSubStr(std::string & mainStr, const std::string & toErase);


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

extern Application* App;