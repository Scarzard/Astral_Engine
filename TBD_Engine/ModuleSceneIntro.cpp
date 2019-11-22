#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "SpacePartition.h"

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

	App->camera->Move(float3(15.0f, 15.0f, 15.0f));
	App->camera->LookAt(float3(0, 0, 0));

	//---- Create Octree -----------------------
	AABB box(float3(0, 0, 0), float3(0, 0, 0)); 
	box.SetNegativeInfinity();
	QuadTree = new Tree(box);

	App->mesh_loader->LoadFile("Assets/Street/Street environment_V01.fbx");
	//App->mesh_loader->LoadFile("Assets/BakerHouse.fbx");
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->LogInConsole("Unloading Intro scene");
	QuadTree->CleanUp();
	root->DeleteGO(root, true);

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
	if (App->gui->ins_window->selected_GO != nullptr && App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		App->gui->ins_window->selected_GO->DeleteGO(App->gui->ins_window->selected_GO, true);
		App->gui->ins_window->selected_GO = nullptr;
		QuadTree->update_tree = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		App->mesh_loader->LoadFile("Assets/BakerHouse.fbx");
	}

	root->Update(dt);

	if (QuadTree->update_tree && !skip_tree)
	{
		QuadTree->UpdateTree();

		//Insert all the contents to the octree
		for (std::vector<ComponentMesh*>::iterator it = static_meshes.begin(); it != static_meshes.end(); it++)
		{
			if((*it)->my_GO->name.find("dummy") == std::string::npos)
				QuadTree->Insert(*it);
		}

		QuadTree->Root->PruneEmptyLeafs();

		QuadTree->update_tree = false;
	}
	skip_tree = false;
	return want_to_quit;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	if (App->gui->conf_window->draw_plane)
	{
		//PLANE -----------------------------
		glLineWidth(2.0f);

		glBegin(GL_LINES);
		glColor3ub(255, 255, 255);
		for (float i = -100; i <= 100; i+=5)
		{
			glVertex3f(i, 0.f, 0.f);
			glVertex3f(i, 0, 100.f);

			glVertex3f(0.f, 0.f, i);
			glVertex3f(100.f, 0, i);

			glVertex3f(i, 0.f, 0.f);
			glVertex3f(i, 0, -100.f);

			glVertex3f(0.f, 0.f, i);
			glVertex3f(-100.f, 0, i);

		}
		glColor3ub(255, 255, 255);
		glEnd();

		
	}

	//Draw GameObjects Recursively
	if (App->camera->obj_camera->active && App->camera->obj_camera->GetComponentCamera()->culling)
	{
		std::vector<GameObject*> tmp;
		
		for (int i = 0; i > static_meshes.size(); ++i)
		{
			tmp.push_back(static_meshes[i]->my_GO);
		}
		
		QuadTree->Intersects(tmp, App->camera->obj_camera->GetComponentCamera()->frustum);
		for (int i = 0; i < tmp.size(); i++)
			if (tmp[i]->active)
				DrawRecursively((tmp[i]));
	}
	else
		DrawRecursively(root);

	
	//Draw Octree Recursively
	if(App->gui->conf_window->draw_quadtree)
		QuadTree->DrawTree(QuadTree->Root);

	glColor3ub(255, 255, 255);

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

void ModuleSceneIntro::LoadPrimitiveMesh(const par_shapes_mesh_s* m)
{
	GameObject* obj = App->scene_intro->CreateGameObject();
	obj->CreateComponent(Component::ComponentType::Mesh);
	obj->CreateComponent(Component::ComponentType::Texture);

	root->SetChild(obj);

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

	//Generate Bounding box for Primitive
	ComponentMesh* tmp = obj->GetComponentMesh();
	tmp->aabb.SetNegativeInfinity();
	tmp->aabb = tmp->aabb.MinimalEnclosingAABB(tmp->vertex, tmp->num_vertex);

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

	LoadPrimitiveMesh(obj);

}

void ModuleSceneIntro::CreateTrefoil(float x, float y, float z, int slices, int stacks, int rad)
{
	//There has to be at least 3 slices and at least 3 stacks, otherwise it won't create the trefoil:(. And radius little than 3 better

	par_shapes_mesh_s* obj = par_shapes_create_trefoil_knot(slices, stacks, rad);

	par_shapes_translate(obj, x, y, z);

	LoadPrimitiveMesh(obj);
}

void ModuleSceneIntro::CreateTorus(float x, float y, float z, int slices, int stacks, int rad)
{
	par_shapes_mesh_s* obj = par_shapes_create_torus(slices, stacks, rad);

	par_shapes_translate(obj, x, y, z);

	LoadPrimitiveMesh(obj);
}

void ModuleSceneIntro::CreateCube(float x, float y, float z, float size)
{
	par_shapes_mesh_s* obj = par_shapes_create_cube();

	par_shapes_scale(obj, size, size, size);
	par_shapes_translate(obj, x, y, z);

	// may not work cause no UV's ?¿
	LoadPrimitiveMesh(obj);
}



