#include "MeshLoader.h"
#include "Application.h"

#include "GameObject.h"
#include "ModuleSceneIntro.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "mmgr/mmgr.h"



MeshLoader::MeshLoader(bool start_enabled) : Module(start_enabled)
{
}

MeshLoader::~MeshLoader()
{}



bool MeshLoader::Init()
{

	// Stream log messages to Debug window 
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool MeshLoader::Start()
{

	return true;
}

update_status MeshLoader::Update(float dt)
{
	
	return UPDATE_CONTINUE;
}

bool MeshLoader::CleanUp()
{
	// detach log stream 
	aiDetachAllLogStreams();



	return true;
}

void MeshLoader::LoadFile(const char* full_path)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	GameObject* Empty = App->scene_intro->CreateGameObject();
	Empty->name = GetNameFromPath(full_path);

	//Child of root node
	App->scene_intro->root->SetChild(Empty);

	if (scene != nullptr && scene->HasMeshes()) //Load sucesful
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			GameObject* obj = App->scene_intro->CreateGameObject();
			Empty->SetChild(obj);

			aiMesh* new_mesh = scene->mMeshes[i];

			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			
			if (path.C_Str() != nullptr)
			{
				std::string directory = GetDirectoryFromPath(full_path);
				directory.append("/");
				directory.append(path.C_Str());

				obj->GetComponentTexture()->texture = App->tex_loader->LoadTextureFromPath(directory.c_str());
			}
			else
			{
				obj->GetComponentTexture()->texture = App->tex_loader->DefaultTexture;
			}
			


			obj->GetComponentMesh()->num_vertex = new_mesh->mNumVertices;
			obj->GetComponentMesh()->vertex = new float3[obj->GetComponentMesh()->num_vertex];

			for (uint i = 0; i < new_mesh->mNumVertices; ++i)
			{
				obj->GetComponentMesh()->vertex[i].x = new_mesh->mVertices[i].x;
				obj->GetComponentMesh()->vertex[i].y = new_mesh->mVertices[i].y;
				obj->GetComponentMesh()->vertex[i].z = new_mesh->mVertices[i].z;
			}

			// copy faces
			if (new_mesh->HasFaces())
			{
				obj->GetComponentMesh()->num_index = new_mesh->mNumFaces * 3;
				obj->GetComponentMesh()->index = new uint[obj->GetComponentMesh()->num_index]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
						App->LogInConsole("WARNING, geometry face with != 3 indices!");
					else
						memcpy(&obj->GetComponentMesh()->index[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}

			if (new_mesh->HasTextureCoords(0))
			{
				obj->GetComponentMesh()->num_tex_coords = obj->GetComponentMesh()->num_vertex;
				obj->GetComponentMesh()->tex_coords = new float[obj->GetComponentMesh()->num_tex_coords * 2];

				for (int i = 0; i < obj->GetComponentMesh()->num_tex_coords; ++i)
				{
					obj->GetComponentMesh()->tex_coords[i * 2] = new_mesh->mTextureCoords[0][i].x;
					obj->GetComponentMesh()->tex_coords[(i * 2) + 1] = new_mesh->mTextureCoords[0][i].y;
				}
			}

			//Generate the buffers 
			App->renderer3D->NewVertexBuffer(obj->GetComponentMesh()->vertex, obj->GetComponentMesh()->num_vertex, obj->GetComponentMesh()->id_vertex);
			App->renderer3D->NewIndexBuffer(obj->GetComponentMesh()->index, obj->GetComponentMesh()->num_index, obj->GetComponentMesh()->id_index);
			//Generate the buffer for texture coords
			App->renderer3D->NewTexBuffer(obj->GetComponentMesh()->tex_coords, obj->GetComponentMesh()->num_tex_coords, obj->GetComponentMesh()->id_tex_coords);
			
			
		}
		aiReleaseImport(scene);
		App->LogInConsole("Succesfully loaded mesh with path: %s", full_path);

	}
	else
		App->LogInConsole("Error loading scene %s", full_path);
}

std::string MeshLoader::GetDirectoryFromPath(std::string path)
{
	std::string directory;
	size_t found = path.find_last_of("/\\");
	directory = path.substr(0, found);

	return directory;
}

std::string MeshLoader::GetNameFromPath(std::string path)
{
	std::string name = path;

	uint num = name.find_last_of("/");
	name = name.substr(num + 1, name.size());

	uint dot = name.find_last_of(".");
	name = name.substr(0, dot);

	return name;
}

