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


	//DRAW CUBE INFO
	uint id_vertices = 0;
	uint id_no_dup_vertices = 0;
	uint id_indices = 0;
};
