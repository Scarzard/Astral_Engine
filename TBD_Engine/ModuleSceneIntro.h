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

	void DrawCube_36v(float x, float y, float z, float size, uint red, uint green, uint blue);
	void DrawCube_8v(float x, float y, float z, float size, uint red, uint green, uint blue);

	void DrawCylinder(int slices, int stacks);


	//DRAW CUBE INFO
	uint id_vertices = 0;
	uint id_no_dup_vertices = 1;
	uint id_indices = 2;

	uint id_cylinder = 3;

	update_status want_to_quit = UPDATE_CONTINUE;
};
