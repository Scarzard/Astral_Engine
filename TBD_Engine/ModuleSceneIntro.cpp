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

bool ModuleSceneIntro::Init()
{
	bool ret = true;

	// needs to be created before Init so other modules can use it
	root = CreateGameObject();
	root->name = "root";

	return ret;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->LogInConsole("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	App->mesh_loader->LoadFile("Assets/BakerHouse.fbx");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->LogInConsole("Unloading Intro scene");
	
	root->CleanUp();
	root->DeleteGO(root);

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject()
{
	std::string Name = "GameObject ";
	Name.append(std::to_string(numGO));
		
	GameObject* GO = new GameObject(Name);
	GO->id = numGO;
	numGO++;

	if(GO->id != 0)
		root->SetChild(GO);

	return GO;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//Trying to delete GO selected if pressed SUPR
	/*if (App->gui->ins_window->selected_GO != nullptr && App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		App->gui->ins_window->selected_GO->DeleteGO(App->gui->ins_window->selected_GO);
		App->gui->ins_window->selected_GO = nullptr;
	}*/

	root->Update(dt);

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


	//Draw GameObjects Recursively
	DrawRecursively(root);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawRecursively(GameObject* GO)
{
	// Not the root and GO is active
	if (GO->id != 0 && GO->active == true)
	{
		glPushMatrix();
		glMultMatrixf((GLfloat*)&GO->GetComponentTransform()->GetGlobalTransform().Transposed());
		App->renderer3D->Draw(GO);
		glPopMatrix();
	}

	if (GO->children.size() > 0)
	{
		for (std::vector<GameObject*>::iterator it = GO->children.begin(); it != GO->children.end(); ++it)
		{
			DrawRecursively(*it);
		}
	}
}

void ModuleSceneIntro::LoadPrimitiveMesh(const par_shapes_mesh_s* m, float x, float y, float z)
{
	GameObject* obj = App->scene_intro->CreateGameObject();
	root->SetChild(obj);

	obj->GetComponentTransform()->position.x = x;
	obj->GetComponentTransform()->position.y = y;
	obj->GetComponentTransform()->position.z = z;

	// VERTEX ----------------
	obj->GetComponentMesh()->num_vertex = m->npoints;
	obj->GetComponentMesh()->vertex = new float3[obj->GetComponentMesh()->num_vertex];

	for (int i = 0; i < obj->GetComponentMesh()->num_vertex; i++)
	{
		int j = i * 3;
		obj->GetComponentMesh()->vertex[i].x = m->points[j];
		obj->GetComponentMesh()->vertex[i].y = m->points[j + 1];
		obj->GetComponentMesh()->vertex[i].z = m->points[j + 2];
	}

	// INDEX ------------
	obj->GetComponentMesh()->num_index = m->ntriangles * 3;
	obj->GetComponentMesh()->index = new uint[obj->GetComponentMesh()->num_index];

	for (int i = 0; i < obj->GetComponentMesh()->num_index; i++)
	{
		obj->GetComponentMesh()->index[i] = (uint)m->triangles[i];
	}

	// TEXTURE ----------------
	obj->GetComponentMesh()->num_tex_coords = m->npoints;
	obj->GetComponentMesh()->tex_coords = new float[obj->GetComponentMesh()->num_tex_coords * 2];

	//Copy the par_shapes texture coordinates
	for (int i = 0; i < obj->GetComponentMesh()->num_tex_coords * 2; ++i)
		obj->GetComponentMesh()->tex_coords[i] = m->tcoords[i];

	//Checkers texture to primitive
	obj->GetComponentTexture()->texture = App->tex_loader->DefaultTexture;

	//Generate the buffers 
	App->renderer3D->NewVertexBuffer(obj->GetComponentMesh()->vertex, obj->GetComponentMesh()->num_vertex, obj->GetComponentMesh()->id_vertex);
	App->renderer3D->NewIndexBuffer(obj->GetComponentMesh()->index, obj->GetComponentMesh()->num_index, obj->GetComponentMesh()->id_index);
	//Generate the buffer for texture coords
	App->renderer3D->NewTexBuffer(obj->GetComponentMesh()->tex_coords, obj->GetComponentMesh()->num_tex_coords, obj->GetComponentMesh()->id_tex_coords);

	App->LogInConsole("Created Primitive with %d vertices and %d indices.", obj->GetComponentMesh()->num_vertex, obj->GetComponentMesh()->num_index);

}

// CREATE SHAPES FUNCTIONS //

void ModuleSceneIntro::CreateSphere(float x, float y, float z, int slices, int stacks)
{
	//There has to be at least 3 slices and at least 3 stacks, otherwise it won't create the sphere :(

	par_shapes_mesh_s* obj = par_shapes_create_parametric_sphere(slices, stacks);

	par_shapes_translate(obj, x, y, z);

	LoadPrimitiveMesh(obj, x,y,z);

}

void ModuleSceneIntro::CreateTrefoil(float x, float y, float z, int slices, int stacks, int rad)
{
	//There has to be at least 3 slices and at least 3 stacks, otherwise it won't create the trefoil:(. And radius little than 3 better

	par_shapes_mesh_s* obj = par_shapes_create_trefoil_knot(slices, stacks, rad);

	par_shapes_translate(obj, x, y, z);

	LoadPrimitiveMesh(obj,x,y,z);
}

void ModuleSceneIntro::CreateTorus(float x, float y, float z, int slices, int stacks, int rad)
{
	par_shapes_mesh_s* obj = par_shapes_create_torus(slices, stacks, rad);

	par_shapes_translate(obj, x, y, z);

	LoadPrimitiveMesh(obj,x,y,z);
}

void ModuleSceneIntro::CreateCube(float x, float y, float z, float size)
{
	par_shapes_mesh_s* obj = par_shapes_create_cube();

	par_shapes_scale(obj, size, size, size);
	par_shapes_translate(obj, x, y, z);

	// may not work cause no UV's ?¿
	LoadPrimitiveMesh(obj,x,y,z);
}



