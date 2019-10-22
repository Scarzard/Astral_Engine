#pragma once
#include "Module.h"
#include "Globals.h"
#include "Shapes.h"
#include "GameObject.h"

class Shapes;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


	//DRAW CUBE INFO
	uint id_vertices = 0;
	uint id_no_dup_vertices = 1;
	uint id_indices = 2;

	update_status want_to_quit = UPDATE_CONTINUE;

	GameObject* CreateGameObject();

	std::vector<GameObject*> GO_list;
};
