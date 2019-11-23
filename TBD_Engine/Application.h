#pragma once

#include <vector>

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEngineUI.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "ModuleFileSystem.h"
#include "TimeManager.h"

enum class ENGINE_STATE
{
	IN_EDITOR = 0,
	PLAY,
	PAUSE
};


class Application
{
public:
	ModuleWindow*			window = nullptr;
	ModuleInput*			input = nullptr;
	ModuleSceneIntro*		scene_intro = nullptr;
	ModuleRenderer3D*		renderer3D = nullptr;
	ModuleCamera3D*			camera = nullptr;
	ModuleEngineUI*			gui = nullptr;
	TimeManager*			time = nullptr;
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

	bool Play();
	void Pause();
	void Stop();

	void ForceEngineState(ENGINE_STATE state = ENGINE_STATE::IN_EDITOR);

	//Request browser
	void OpenLink(const char* link);

	void LogInConsole(const char* format, ...);
	std::vector<std::string>	Logs_Console;

	ENGINE_STATE GetState();
	const std::string GetNameFromPath(std::string path);
	const std::string GetDirectoryFromPath(std::string path);
	const std::string GetFileExtension(const std::string FileName);
	void eraseSubStr(std::string & mainStr, const std::string & toErase);

	float GetDT();
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
	float	using_dt = 0.0f;

	ENGINE_STATE state = ENGINE_STATE::IN_EDITOR;

};

extern Application* App;