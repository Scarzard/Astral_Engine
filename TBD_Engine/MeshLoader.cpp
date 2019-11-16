#include "MeshLoader.h"
#include "Application.h"

#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "Importer.h"

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
	aiNode* root_node = scene->mRootNode;

	GameObject* Empty = App->scene_intro->CreateGameObject();
	Empty->name = App->GetNameFromPath(full_path);

	aiVector3D translation, scaling;
	aiQuaternion rotation;

	root_node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 s(1, 1, 1);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	Empty->GetComponentTransform()->position = pos;
	Empty->GetComponentTransform()->scale = s;
	Empty->GetComponentTransform()->rotation_quat = rot;

	Empty->GetComponentTransform()->UpdateLocalTransform();

	//Child of root node
	App->scene_intro->root->SetChild(Empty);

	if (scene != nullptr && scene->HasMeshes()) //Load sucesful
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			GameObject* obj = App->scene_intro->CreateGameObject();
			
			Empty->SetChild(obj);

			aiNode* node = root_node->mChildren[i];
			obj->name = node->mName.C_Str();
			App->eraseSubStr(obj->name, "_$AssimpFbx$_Translation");
			

			aiVector3D translation, scaling;
			aiQuaternion rotation;

			node->mTransformation.Decompose(scaling, rotation, translation);

			float3 pos2(translation.x, translation.y, translation.z);
			//float3 pos2(0, 0, 0);
			//float3 s2(scaling.x, scaling.y, scaling.z);
			float3 s2(1, 1, 1);
			Quat rot2(0, rotation.y, rotation.z, rotation.w);

			obj->GetComponentTransform()->position = pos2;
			obj->GetComponentTransform()->scale = s2;
			obj->GetComponentTransform()->rotation_quat = rot2;

			obj->GetComponentTransform()->UpdateLocalTransform();

			aiMesh* new_mesh = scene->mMeshes[i];

			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			
			if (path.C_Str() != nullptr)
			{
				std::string directory = App->GetDirectoryFromPath(full_path);
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

			//normals

			obj->GetComponentMesh()->face_center = new float3[obj->GetComponentMesh()->num_index];
			obj->GetComponentMesh()->face_normal = new float3[obj->GetComponentMesh()->num_index];
			obj->GetComponentMesh()->num_normals = obj->GetComponentMesh()->num_index / 3;
			for (uint j = 0; j < obj->GetComponentMesh()->num_index / 3; ++j)
			{
				float3 face_A, face_B, face_C;

				face_A = obj->GetComponentMesh()->vertex[obj->GetComponentMesh()->index[j * 3]];
				face_B = obj->GetComponentMesh()->vertex[obj->GetComponentMesh()->index[(j * 3) + 1]];
				face_C = obj->GetComponentMesh()->vertex[obj->GetComponentMesh()->index[(j * 3) + 2]];


				obj->GetComponentMesh()->face_center[j] = (face_A + face_B + face_C) / 3;


				float3 edge1 = face_B - face_A;
				float3 edge2 = face_C - face_A;

				obj->GetComponentMesh()->face_normal[j] = Cross(edge1, edge2);
				obj->GetComponentMesh()->face_normal[j].Normalize();
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
			
			Importer ex;
			std::string output_file;
			const char* name = obj->name.c_str();
			ex.Export(name, output_file, obj->GetComponentMesh());
		}
		aiReleaseImport(scene);
		App->LogInConsole("Succesfully loaded mesh with path: %s", full_path);

	}
	else
		App->LogInConsole("Error loading scene %s", full_path);
}





