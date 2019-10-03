#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:

	float vertices[300];
	int indices[300];

	int num_vertices = 0;
	int num_indices = 0;

};
