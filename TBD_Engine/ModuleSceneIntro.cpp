#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "SpacePartition.h"
#include "ModuleCamera3D.h"
#include "MeshLoader.h"
#include "ModuleEngineUI.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleResources.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <map>

#include "par/par_shapes.h"
#include "Math.h"

#include <fstream>
#include <iomanip>

#include "mmgr/mmgr.h"



ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

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

	App->mesh_loader->LoadFile("Assets/FBX/Street/Street environment_V01.fbx");
	//App->mesh_loader->LoadFile("Assets/FBX/BakerHouse/BakerHouse.fbx");
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
		if (App->gui->ins_window->selected_GO != root)
		{
			App->gui->ins_window->selected_GO->DeleteGO(App->gui->ins_window->selected_GO, true);
			App->gui->ins_window->selected_GO = nullptr;
			QuadTree->update_tree = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) //Test
	{
		App->gui->save_scene_pop = true;
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

		//QuadTree->Root->PruneEmptyLeafs();

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
	//Toggle Frustum culling 
	if (App->camera->obj_camera->components.size() > 0)
	{
		if (App->camera->obj_camera->GetComponentCamera() != nullptr)
		{
			if (App->camera->obj_camera->components.size() > 0 && App->camera->obj_camera->active && App->camera->obj_camera->GetComponentCamera()->culling)
			{
				// Static Frustum Culling
				std::vector<GameObject*> tmp_static_go;
				QuadTree->Intersects(tmp_static_go, App->camera->obj_camera->GetComponentCamera()->frustum);
				for (int i = 0; i < tmp_static_go.size(); i++)
					if (tmp_static_go[i]->active)
						DrawRecursively(tmp_static_go[i]);
			}
			else
				DrawRecursively(root);
		}
		else
			DrawRecursively(root);
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

GameObject* ModuleSceneIntro::CollectHits()
{
	GameObject* ret = nullptr;

	LineSegment ray = *App->camera->GetLastRay();

	std::map<float, GameObject*> objects;
	App->scene_intro->QuadTree->Intersects(objects, ray);

	std::map<float, GameObject*>::iterator it;
	for (it = objects.begin(); it != objects.end(); ++it)
	{
		ComponentMesh* mesh = (*it).second->GetComponentMesh();
		if (mesh)
		{
			LineSegment localRay = ray;
			localRay.Transform(it->second->GetComponentTransform()->GetGlobalTransform().Inverted());
			for (uint tmp = 0; tmp < mesh->res_mesh->num_index; tmp += 3)
			{
				uint indexA = mesh->res_mesh->index[tmp] * 3;
				float3 a(mesh->res_mesh->vertex[indexA]);

				uint indexB = mesh->res_mesh->index[tmp + 1] * 3;
				float3 b(mesh->res_mesh->vertex[indexB]);

				uint indexC = mesh->res_mesh->index[tmp + 2] * 3;
				float3 c(mesh->res_mesh->vertex[indexC]);
				for (uint j = 0; j < mesh->res_mesh->num_index / 3; j++)
				{
					float3 a = mesh->res_mesh->vertex[mesh->res_mesh->index[j * 3]];
					float3 b = mesh->res_mesh->vertex[mesh->res_mesh->index[(j * 3) + 1]];
					float3 c = mesh->res_mesh->vertex[mesh->res_mesh->index[(j * 3) + 2]];
					// --- Create Triangle given three vertices ---
					Triangle triangle(a, b, c);

					if (localRay.Intersects(triangle, nullptr, nullptr))
					{
						return (GameObject*)it->second;
					}
				}
			}
		}
	}

	return ret;
}


void ModuleSceneIntro::LoadScene(std::string scene_name)
{
	// Delete the current scene
	for (int i = 0; root->children.size() != 0;)
	{
		root->children[i]->DeleteGO(root->children[i], true);
		App->gui->ins_window->selected_GO = nullptr;
		QuadTree->update_tree = true;
	}
	root->children.clear();
	numGO = 0;

	json file;
	std::string path = ASSETS_SCENES_FOLDER + scene_name;

	std::ifstream stream;
	stream.open(path);
	file = json::parse(stream);
	stream.close();

	std::vector<GameObject*> objects;
	for (json::iterator it = file.begin(); it != file.end(); ++it)
	{
		std::string name = it.key().data();
		GameObject* obj = CreateGameObject();
		if (name.compare("root") != 0)
		{
			
			obj->name = name;
			std::string uid = file[it.key()]["UUID"];
			obj->UUID = std::stoi(uid);

			if (name == "Main Camera")
			{
				App->camera->obj_camera = obj;
			}

			json components = file[it.key()]["Components"];
			ComponentTransform* transform = obj->GetComponentTransform();

			std::string posx = components["Transform"]["PositionX"];
			std::string posy = components["Transform"]["PositionY"];
			std::string posz = components["Transform"]["PositionZ"];

			std::string rotx = components["Transform"]["RotationX"];
			std::string roty = components["Transform"]["RotationY"];
			std::string rotz = components["Transform"]["RotationZ"];

			std::string sx = components["Transform"]["ScaleX"];
			std::string sy = components["Transform"]["ScaleY"];
			std::string sz = components["Transform"]["ScaleZ"];

			
			transform->SetPosition(float3(std::stof(posx), std::stof(posy), std::stof(posz)));
			transform->SetEulerRotation(float3(std::stof(rotx), std::stof(roty), std::stof(rotz)));
			transform->SetScale(float3(std::stof(sx), std::stof(sy), std::stof(sz)));

			for (json::iterator comp = components.begin(); comp != components.end(); ++comp)
			{
				std::string type = comp.key();

				if (type.compare("Mesh") == 0)
				{
					obj->CreateComponent(Component::ComponentType::Mesh);
					ComponentMesh* mesh = obj->GetComponentMesh();

					std::string comp_id = components[comp.key()]["UUID"];
					uint mesh_id = std::stoi(comp_id);
					mesh->UUID = mesh_id;
					
					std::string name = components[comp.key()]["ResourceName"];

					mesh->res_mesh = (ResourceMesh*)App->resources->Get(App->resources->IsResourceInLibrary(name.c_str()));
					mesh->res_mesh->UpdateNumReference();

					mesh->UpdateAABB();
					mesh->UpdateGlobalAABB();
					App->scene_intro->static_meshes.push_back(mesh);
					App->scene_intro->meshes.push_back(mesh);

				}

				if (type.compare("Texture") == 0)
				{
					obj->CreateComponent(Component::ComponentType::Texture);
					ComponentTexture* tex = obj->GetComponentTexture();

					std::string comp_id = components[comp.key()]["UUID"];
					uint mesh_id = std::stoi(comp_id);
					tex->UUID = mesh_id;

					std::string tex_name = components[comp.key()]["ResourceName"];
					tex_name = App->GetNameFromPath(tex_name);
					tex->res_texture = (ResourceTexture*)App->resources->Get(App->resources->IsResourceInLibrary(tex_name.c_str()));
					tex->res_texture->UpdateNumReference();
				}
			}


			objects.push_back(obj);
		}

		
	}

	// --- Parent GO's ---
	for (uint i = 0; i < objects.size(); ++i)
	{
		std::string parent_uid = file[objects[i]->name]["ParentUUID"];
		uint p_uid = std::stoi(parent_uid);

		for (uint j = 0; j < objects.size(); ++j)
		{
			if (p_uid == objects[j]->UUID)
			{
				objects[j]->SetChild(objects[i]);
				continue;
			}
		}
	}
}

void ModuleSceneIntro::LoadGameObjects(nlohmann::json & scene, GameObject * Root)
{
	Root->Load(Root->id, scene);

	for (int i = 0; i < Root->children.size(); ++i)
		LoadGameObjects(scene, Root->children[i]);
}

void ModuleSceneIntro::SaveScene(std::string scene_name)
{
	// Create auxiliar file
	json scene;
	std::string full_path = ASSETS_SCENES_FOLDER + scene_name + ".json";

	SaveGameObjects(scene, root);
	// Create the stream and open the file
	std::ofstream stream;
	stream.open(full_path);
	stream << std::setw(4) << scene << std::endl;
	stream.close();
}

void ModuleSceneIntro::SaveGameObjects(nlohmann::json  &scene, GameObject* GO)
{
	GO->Save(GO->id, scene);

	for (int i = 0; i < GO->children.size(); ++i)
		SaveGameObjects(scene, GO->children[i]);
}

void ModuleSceneIntro::LoadPrimitiveMesh(const par_shapes_mesh_s* m)
{
	GameObject* obj = App->scene_intro->CreateGameObject();
	obj->CreateComponent(Component::ComponentType::Mesh);
	obj->CreateComponent(Component::ComponentType::Texture);

	root->SetChild(obj);

	// VERTEX ----------------
	obj->GetComponentMesh()->res_mesh->num_vertex = m->npoints;
	obj->GetComponentMesh()->res_mesh->vertex = new float3[obj->GetComponentMesh()->res_mesh->num_vertex];

	for (int i = 0; i < obj->GetComponentMesh()->res_mesh->num_vertex; i++)
	{
		int j = i * 3;
		obj->GetComponentMesh()->res_mesh->vertex[i].x = m->points[j];
		obj->GetComponentMesh()->res_mesh->vertex[i].y = m->points[j + 1];
		obj->GetComponentMesh()->res_mesh->vertex[i].z = m->points[j + 2];
	}

	// INDEX ------------
	obj->GetComponentMesh()->res_mesh->num_index = m->ntriangles * 3;
	obj->GetComponentMesh()->res_mesh->index = new uint[obj->GetComponentMesh()->res_mesh->num_index];

	for (int i = 0; i < obj->GetComponentMesh()->res_mesh->num_index; i++)
	{
		obj->GetComponentMesh()->res_mesh->index[i] = (uint)m->triangles[i];
	}

	// TEXTURE ----------------
	obj->GetComponentMesh()->res_mesh->num_tex_coords = m->npoints;
	obj->GetComponentMesh()->res_mesh->tex_coords = new float[obj->GetComponentMesh()->res_mesh->num_tex_coords * 2];

	//Copy the par_shapes texture coordinates
	for (int i = 0; i < obj->GetComponentMesh()->res_mesh->num_tex_coords * 2; ++i)
		obj->GetComponentMesh()->res_mesh->tex_coords[i] = m->tcoords[i];

	//Checkers texture to primitive
	//obj->GetComponentTexture()->res_texture = App->tex_loader->DefaultTexture; //TODO make default texture a resource

	//Generate Bounding box for Primitive
	ComponentMesh* tmp = obj->GetComponentMesh();
	tmp->aabb.SetNegativeInfinity();
	tmp->aabb = tmp->aabb.MinimalEnclosingAABB(tmp->res_mesh->vertex, tmp->res_mesh->num_vertex);

	//Generate the buffers 
	App->renderer3D->NewVertexBuffer(obj->GetComponentMesh()->res_mesh->vertex, obj->GetComponentMesh()->res_mesh->num_vertex, obj->GetComponentMesh()->res_mesh->id_vertex);
	App->renderer3D->NewIndexBuffer(obj->GetComponentMesh()->res_mesh->index, obj->GetComponentMesh()->res_mesh->num_index, obj->GetComponentMesh()->res_mesh->id_index);
	//Generate the buffer for texture coords
	App->renderer3D->NewTexBuffer(obj->GetComponentMesh()->res_mesh->tex_coords, obj->GetComponentMesh()->res_mesh->num_tex_coords, obj->GetComponentMesh()->res_mesh->id_tex_coords);

	App->LogInConsole("Created Primitive with %d vertices and %d indices.", obj->GetComponentMesh()->res_mesh->num_vertex, obj->GetComponentMesh()->res_mesh->num_index);

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



