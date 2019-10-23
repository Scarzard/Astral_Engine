#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "par/par_shapes.h"
#include "Math.h"

#include "mmgr/mmgr.h"



ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->LogInConsole("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	uint HouseTexture = App->tex_loader->LoadTextureFromPath("Assets/Baker_house.png");
	App->mesh_loader->LoadFile("Assets/BakerHouse.fbx");

	for (std::vector<GameObject*>::iterator iterator = GO_list.begin(); iterator != GO_list.end(); iterator++)
	{
		(*iterator)->GetComponentMesh()->Texture = HouseTexture; //just to test
	}
	
	/*Shapes* object = new Shapes();
	Shapes* object2 = new Shapes();
	Shapes* object3 = new Shapes();

	object->CreateSphere(1, 1, -5, 50, 20);
	object2->CreateTrefoil(1,5,-5,50,50, 1);
	object3->CreateTorus(5, 1, -5, 50, 50, 1);*/

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->LogInConsole("Unloading Intro scene");
	

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject()
{
	std::string Name = "GameObject";
	Name.append(std::to_string(GO_list.size()));

	
	GameObject* GO = new GameObject(Name.data());
	GO_list.push_back(GO);

	return GO;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	

	return want_to_quit;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	//PLANE -----------------------------
	glLineWidth(2.0f);

	glBegin(GL_LINES);
	glColor3ub(255, 255, 255);
	for (float i = -10; i <= 10; ++i)
	{
		glVertex3f(i, 0.f, 0.f);
		glVertex3f(i, 0, 10.f);

		glVertex3f(0.f, 0.f, i);
		glVertex3f(10.f, 0, i);

		glVertex3f(i, 0.f, 0.f);
		glVertex3f(i, 0, -10.f);

		glVertex3f(0.f, 0.f, i);
		glVertex3f(-10.f, 0, i);
	}
	glEnd();

	// AXIS ------------------------
	glLineWidth(4.0f);

	glBegin(GL_LINES);
	//Y
	glColor3ub(0, 255, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glEnd();

	glBegin(GL_LINES);
	//X
	glColor3ub(255, 0, 0);
	glVertex3f(0.f, 0.001f, 0.f);
	glVertex3f(1.f, 0.001f, 0.f);
	glEnd();

	glBegin(GL_LINES);
	//Z
	glColor3ub(0, 0, 255);
	glVertex3f(0.f, 0.001f, 0.f);
	glVertex3f(0.f, 0.001f, 1.f);
	glEnd();

	glColor3ub(255, 255, 255);


	//Draw meshes
	for (std::vector<GameObject*>::iterator iterator = GO_list.begin(); iterator != GO_list.end(); iterator++)
	{
		App->renderer3D->Draw((*iterator)->GetComponentMesh());
	}

	return UPDATE_CONTINUE;
}




