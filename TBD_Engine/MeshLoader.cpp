#include "MeshLoader.h"
#include "Application.h"


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


	return true;
}

bool MeshLoader::Start()
{
	// Stream log messages to Debug window 
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

update_status MeshLoader::Update(float dt)
{
	//Draw meshes
	for (int i = 0; i < LoadedMeshes.size(); ++i)
		App->renderer3D->Draw(LoadedMeshes[i]);

	return UPDATE_CONTINUE;
}

bool MeshLoader::CleanUp()
{
	// detach log stream 
	aiDetachAllLogStreams();

	LoadedMeshes.clear();

	return true;
}

void MeshLoader::LoadFile(const char* full_path)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes()) //Load sucesful
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			MeshInfo* m = new MeshInfo;

			aiMesh* new_mesh = scene->mMeshes[i];

			m->num_vertex = new_mesh->mNumVertices;
			m->vertex = new float[m->num_vertex * 3];
			memcpy(m->vertex, new_mesh->mVertices, sizeof(float) * m->num_vertex * 3);
			App->LogInConsole("New mesh with %d vertices", m->num_vertex);

			// copy faces
			if (new_mesh->HasFaces())
			{
				m->num_index = new_mesh->mNumFaces * 3;
				m->index = new uint[m->num_index]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
						App->LogInConsole("WARNING, geometry face with != 3 indices!");
					else
						memcpy(&m->index[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}

			//Generate the buffers 
			App->renderer3D->NewVertexBuffer(m->vertex, m->num_vertex, m->id_vertex);
			App->renderer3D->NewIndexBuffer(m->index, m->num_index, m->id_index);

			//Add Loaded mesh to the array f meshes
			LoadedMeshes.push_back(m);
		}
		aiReleaseImport(scene);


	}
	else
		App->LogInConsole("Error loading scene %s", full_path);
}