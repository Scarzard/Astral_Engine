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
	//Draw meshes
	for (int i = 0; i < LoadedMeshes.size(); ++i)
		App->renderer3D->Draw(LoadedMeshes[i]);

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
	
	if (scene != nullptr && scene->HasMeshes()) //Load sucesful
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			MeshInfo* m = new MeshInfo;

			aiMesh* new_mesh = scene->mMeshes[i];

			m->num_vertex = new_mesh->mNumVertices;
			m->vertex = new float[new_mesh->mNumVertices];

			for (uint i = 0; i < new_mesh->mNumVertices; ++i)
			{
				m->vertex[i] = new_mesh->mVertices[i].x;
				m->vertex[i + 1] = new_mesh->mVertices[i].y;
				m->vertex[i] = new_mesh->mVertices[i].z;
			}

			// copy faces
			if (new_mesh->HasFaces())
			{
				m->num_index = new_mesh->mNumFaces * 3;
				m->index = new uint[m->num_index]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					const aiFace& face = new_mesh->mFaces[i];

					assert(face.mNumIndices == 3);

					m->index[i * 3] = face.mIndices[0];
					m->index[i * 3 + 1] = face.mIndices[1];
					m->index[i * 3 + 2] = face.mIndices[2];
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