#include "MeshLoader.h"
#include "Application.h"

#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "SpacePartition.h"
#include "ModuleRenderer3D.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ResourceAnimation.h"
#include "ResourceBone.h"
#include "ModuleFileSystem.h"

#include <fstream>
#include <iomanip>

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
	if (scene != nullptr) //Load sucesful
	{

		std::vector<aiMesh*> mesh_collector;
		std::vector<GameObject*> go_collector;
		aiNode* root_node = scene->mRootNode;

		GameObject* Empty = App->scene_intro->CreateGameObject();
		Empty->name = App->GetNameFromPath(full_path);

		std::string output_file;
		//ex.Export(Empty->name.c_str(), output_file, Empty->GetComponentTransform());

		//Child of root node
		App->scene_intro->root->SetChild(Empty);

		if (root_node->mNumChildren > 0)
			for (int i = 0; i < root_node->mNumChildren; ++i)
			{
				LoadNode(scene, root_node->mChildren[i], Empty, full_path, output_file, mesh_collector, go_collector);
			}


		LoadBones(mesh_collector, go_collector, Empty);

		if (scene->HasAnimations() == true)
		{
			for (int i = 0; i < scene->mNumAnimations; i++)
			{
				// Not checking if it is on memory

				ResourceAnimation* anim = (ResourceAnimation*)App->resources->NewResource(Resource::RES_TYPE::ANIMATION);

				anim->name = scene->mAnimations[i]->mName.C_Str();
				anim->duration = scene->mAnimations[i]->mDuration;
				anim->ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
				anim->file = full_path;

				anim->numChannels = scene->mAnimations[i]->mNumChannels;
				anim->channels = new Channel[anim->numChannels];
				for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
				{
					LoadChannel(scene->mAnimations[i]->mChannels[j], anim->channels[j]);
				}

				Empty->CreateComponent(Component::ComponentType::Animation);
				Empty->GetComponentAnimation()->res_anim = anim;

			}
		}

		aiReleaseImport(scene);
		App->LogInConsole("Succesfully loaded mesh with path: %s", full_path);

		if(App->scene_intro->QuadTree != nullptr) //if quadtree is created
			App->scene_intro->QuadTree->update_tree = true;
	}
	else
	{
		App->LogInConsole("Error loading scene %s", full_path);
	}

}

void MeshLoader::LoadNode(const aiScene * scene, aiNode * Node, GameObject* parent, const char* full_path, std::string output_file, std::vector<aiMesh*>& mesh_collect, std::vector<GameObject*>& go_collect)
{
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	Node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos2(translation.x, translation.y, translation.z);
	float3 s2(1, 1, 1);
	Quat rot2(rotation.x, rotation.y, rotation.z, rotation.w);

	std::string node_name = Node->mName.C_Str();

	bool dummyFound = true;
	while (dummyFound)//Skipp all dummy modules. Assimp loads this fbx nodes to stack all transformations
	{
		//All dummy modules have one children (next dummy module or last module containing the mesh)
		if (node_name.find("_$AssimpFbx$_") != std::string::npos && Node->mNumChildren == 1)
		{
			//Dummy module have only one child node, so we use that one as our next GameObject
			Node = Node->mChildren[0];

			// Accumulate transform 
			Node->mTransformation.Decompose(scaling, rotation, translation);
			pos2 += float3(translation.x, translation.y, translation.z);
			s2 = float3(s2.x * scaling.x, s2.y * scaling.y, s2.z * scaling.z);
			rot2 = rot2 * Quat(rotation.x, rotation.y, rotation.z, rotation.w);

			node_name = Node->mName.C_Str();

			//if we find a dummy node we "change" our current node into the dummy one and search
			//for other dummy nodes inside that one.
			dummyFound = true;
		}
		else
			dummyFound = false;
	}

	GameObject* obj = App->scene_intro->CreateGameObject();
	ComponentTransform* transf = obj->GetComponentTransform();

	transf->position = pos2;
	transf->scale = s2;
	transf->rotation_quat = rot2;

	transf->UpdateLocalTransform();

	parent->SetChild(obj);
	obj->name = node_name;

	for (int i = 0; i < Node->mNumMeshes; ++i)
	{
		
		aiMesh* new_mesh = scene->mMeshes[Node->mMeshes[i]];

		GameObject* child = nullptr;
		if (Node->mNumMeshes > 1)
		{
			node_name = new_mesh->mName.C_Str();
			if (node_name == "")
				node_name = obj->name + "dummy";
			if (i > 0)
				node_name = obj->name + "_submesh";
			child = App->scene_intro->CreateGameObject();
			obj->SetChild(child);
			child->name = node_name;
		}
		else
		{
			//If node has only 1 mesh child is the parent itself 
			child = obj;
		}

		child->CreateComponent(Component::ComponentType::Mesh);
		ComponentMesh* mesh = child->GetComponentMesh();

		//If this mesh is inmemory 
		if (App->resources->IsResourceInLibrary(child->name.c_str()) != 0)
		{
			mesh->res_mesh = (ResourceMesh*)App->resources->Get(App->resources->IsResourceInLibrary(child->name.c_str()));
			if (mesh->res_mesh != nullptr)
				mesh->res_mesh->UpdateNumReference();
		}
		else
		{
			ResourceMesh* resource_mesh = (ResourceMesh*)App->resources->NewResource(Resource::RES_TYPE::MESH);

			uint num_vertex = new_mesh->mNumVertices;
			float3* vertex = new float3[num_vertex];

			for (uint i = 0; i < new_mesh->mNumVertices; ++i)
			{
				vertex[i].x = new_mesh->mVertices[i].x;
				vertex[i].y = new_mesh->mVertices[i].y;
				vertex[i].z = new_mesh->mVertices[i].z;
			}

			bool indices3 = true;
			uint num_index = 0;
			uint* index = nullptr;
			// copy faces
			if (new_mesh->HasFaces())
			{
				num_index = new_mesh->mNumFaces * 3;
				index = new uint[num_index]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
					{
						indices3 = false;
						App->LogInConsole("WARNING, mesh %s geometry face with != 3 indices!", obj->name.c_str());
					}
					else
						memcpy(&index[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

			}

			if (new_mesh->HasBones())
			{
				mesh_collect.push_back(new_mesh);
				go_collect.push_back(child);
			}

			float3* face_center = nullptr;
			float3* face_normal = nullptr;
			uint num_normals = 0;
			//normals
			if (new_mesh->HasNormals() && indices3)
			{
				face_center = new float3[num_index];
				face_normal = new float3[num_index];
				num_normals = num_index / 3;
				for (uint j = 0; j < num_index / 3; ++j)
				{
					float3 face_A, face_B, face_C;


					face_A = vertex[index[j * 3]];
					face_B = vertex[index[(j * 3) + 1]];
					face_C = vertex[index[(j * 3) + 2]];


					face_center[j] = (face_A + face_B + face_C) / 3;


					float3 edge1 = face_B - face_A;
					float3 edge2 = face_C - face_A;

					face_normal[j] = Cross(edge1, edge2);
					face_normal[j].Normalize();
					face_normal[j] *= 0.15f;

				}
			}

			uint num_tex_coords = 0;
			float* tex_coords = nullptr;
			if (new_mesh->HasTextureCoords(0))
			{
				num_tex_coords = num_vertex;
				tex_coords = new float[num_tex_coords * 2];

				for (int i = 0; i < num_tex_coords; ++i)
				{
					tex_coords[i * 2] = new_mesh->mTextureCoords[0][i].x;
					tex_coords[(i * 2) + 1] = new_mesh->mTextureCoords[0][i].y;
				}
			}

			//mesh->res_mesh = mesh_resource;

			//Create .mesh
			Export(child->name.c_str(), resource_mesh->exported_file, num_index, index, num_vertex, vertex, num_normals, face_center, face_normal, num_tex_coords, tex_coords);
			resource_mesh->file = full_path;

			mesh->res_mesh = resource_mesh;
			if(mesh->res_mesh != nullptr)
				mesh->res_mesh->UpdateNumReference();

			if (index != nullptr)
			{
				delete[] index;
				index = nullptr;
			}

			if (vertex != nullptr)
			{
				delete[] vertex;
				vertex = nullptr;
			}

			if (tex_coords != nullptr)
			{
				delete[] tex_coords;
				tex_coords = nullptr;
			}

			if (face_center != nullptr)
			{
				delete[] face_center;
				face_center = nullptr;
			}

			if (face_normal != nullptr)
			{
				delete[] face_normal;
				face_normal = nullptr;
			}
		}
		aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
		uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		if (path.C_Str() != nullptr && path.length > 0)
		{
			std::string name = App->GetNameFromPath(path.C_Str());
			std::string ext = App->GetFileExtension(path.C_Str());

			child->CreateComponent(Component::ComponentType::Texture);
			std::string directory = App->GetDirectoryFromPath(full_path);
			directory.append("/");
			directory.append(name + "." + ext);

			child->GetComponentTexture()->res_texture = (ResourceTexture*)App->resources->Get(App->resources->GetNewFile(directory.c_str()));
			if(child->GetComponentTexture()->res_texture != nullptr) 
				child->GetComponentTexture()->res_texture->UpdateNumReference();
		}
		
		//Initiate bounding box when creating our mesh
		mesh->UpdateAABB();
		mesh->UpdateGlobalAABB();

		App->scene_intro->static_meshes.push_back(mesh);
		App->scene_intro->meshes.push_back(mesh);
	}
	
	
	if (Node->mNumChildren > 0)
		for (int i = 0; i < Node->mNumChildren; ++i)
		{
			LoadNode(scene, Node->mChildren[i], obj, full_path, output_file, mesh_collect, go_collect);
		}
}



bool MeshLoader::Export(const char * name, std::string & output_file, uint num_index, uint* index, uint num_vertex, float3* vertex, uint num_normals, float3* face_center, float3* face_normal, uint num_tex_coords, float* tex_coords) // Create .mesh own file format
{
	bool ret = false;

	// amount of indices / vertices / normals / texture_coords / AABB
	uint ranges[4] = { num_index, num_vertex,  num_normals, num_tex_coords };

	uint size = sizeof(ranges) + sizeof(uint) * num_index + sizeof(float) * num_vertex * 3 + sizeof(float) * num_normals * 3 * 2 + sizeof(float) * num_tex_coords * 2;

	char* data = new char[size]; // Allocate
	char* cursor = data;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store indices
	bytes = sizeof(uint) *num_index;
	memcpy(cursor, index, bytes);

	cursor += bytes; // Store Vertices
	bytes = sizeof(float) *num_vertex * 3;
	memcpy(cursor, vertex, bytes);

	cursor += bytes; // Store Normals Starting Point
	bytes = sizeof(float) * num_normals * 3;
	memcpy(cursor, face_center, bytes);

	cursor += bytes; // Store Normals Vector
	bytes = sizeof(float) * num_normals * 3;
	memcpy(cursor, face_normal, bytes);

	cursor += bytes; // Store Texture Coordinates
	bytes = sizeof(float) *num_tex_coords * 2;
	memcpy(cursor, tex_coords, bytes);


	ret = App->file_system->SaveUnique(output_file, data, size, LIBRARY_MESH_FOLDER, name, "mesh");

	if (!ret)
		App->LogInConsole("Failed exporting %s.mesh into Library/mesh floder", name);

	if (data)
	{
		delete[] data;
		data = nullptr;
		cursor = nullptr;
	}

	return ret;
}

bool MeshLoader::Load(ResourceMesh* mesh)
{
	bool ret = true;

	char* buffer = nullptr;
	App->file_system->Load(mesh->exported_file.c_str(), &buffer); //put data file in buffer

	if (buffer)
	{
		char* cursor = buffer;

		// amount of indices / vertices / normals / texture_coords
		uint ranges[4];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		mesh->num_index = ranges[0];
		mesh->num_vertex = ranges[1];
		mesh->num_normals = ranges[2];
		mesh->num_tex_coords = ranges[3];

		cursor += bytes; // Load indices
		bytes = sizeof(uint) * mesh->num_index;
		mesh->index = new uint[mesh->num_index];
		memcpy(mesh->index, cursor, bytes);

		cursor += bytes; // Load vertex
		bytes = sizeof(float) * mesh->num_vertex * 3;
		mesh->vertex = new float3[mesh->num_vertex];
		memcpy(mesh->vertex, cursor, bytes);

		cursor += bytes; // Load Normals Starting Point
		bytes = sizeof(float) * mesh->num_normals * 3;
		mesh->face_center = new float3[mesh->num_normals];
		memcpy(mesh->face_center, cursor, bytes);

		cursor += bytes; // Load Normals Vector
		bytes = sizeof(float) * mesh->num_normals * 3;
		mesh->face_normal = new float3[mesh->num_normals];
		memcpy(mesh->face_normal, cursor, bytes);

		cursor += bytes; // Load Texture Coordinates
		bytes = sizeof(float) * mesh->num_tex_coords * 2;
		mesh->tex_coords = new float[mesh->num_tex_coords * 2];
		memcpy(mesh->tex_coords, cursor, bytes);

		//Generate the buffers 
		App->renderer3D->NewVertexBuffer(mesh->vertex, mesh->num_vertex, mesh->id_vertex);
		App->renderer3D->NewIndexBuffer(mesh->index, mesh->num_index, mesh->id_index);
		//Generate the buffer for texture coords
		App->renderer3D->NewTexBuffer(mesh->tex_coords, mesh->num_tex_coords, mesh->id_tex_coords);

		RELEASE_ARRAY(buffer);
		cursor = nullptr;

		ret = true;
	}
	return ret;
}

void MeshLoader::FillMap(std::map<std::string, GameObject*>& map, GameObject* root)
{
	map[root->name.c_str()] = root;
	for (int i = 0; i < root->children.size(); i++)
	{
		FillMap(map, root->children[i]);
	}
}

void MeshLoader::LoadBoneData(const aiBone* bone, ResourceBone* res_bone, uint mesh_id)
{
	res_bone->meshID = mesh_id;
	res_bone->NumWeights = bone->mNumWeights;
	res_bone->weight = new float[res_bone->NumWeights];
	res_bone->index_weight = new uint[res_bone->NumWeights];
	for (int i = 0; i < res_bone->NumWeights; i++)
	{
		res_bone->weight[i] = bone->mWeights[i].mWeight;
		res_bone->index_weight[i] = bone->mWeights[i].mVertexId;
	}

	res_bone->matrix = float4x4(bone->mOffsetMatrix.a1, bone->mOffsetMatrix.a2, bone->mOffsetMatrix.a3, bone->mOffsetMatrix.a4,
							   bone->mOffsetMatrix.b1, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.b4,
							   bone->mOffsetMatrix.c1, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.c4,
							   bone->mOffsetMatrix.d1, bone->mOffsetMatrix.d2, bone->mOffsetMatrix.d3, bone->mOffsetMatrix.d4);

}

void MeshLoader::LoadBones(std::vector<aiMesh*> mesh_collect, std::vector<GameObject*> go_collect, GameObject* root)
{
	std::map<std::string, GameObject*> go_map;
	FillMap(go_map, root);

	for (int i = 0; i < mesh_collect.size(); i++)
	{
		for (int j = 0; j < mesh_collect[i]->mNumBones; j++)
		{
			ResourceBone* rBone = (ResourceBone*)App->resources->NewResource(Resource::BONE);
			LoadBoneData(mesh_collect[i]->mBones[j], rBone, go_collect[i]->GetComponentMesh()->res_mesh->GetUUID());
			std::map<std::string, GameObject*>::iterator bone = go_map.find(mesh_collect[i]->mBones[j]->mName.C_Str());
			if (bone != go_map.end())
			{
				ComponentBone* c_bone = (ComponentBone*)bone->second->CreateComponent(Component::ComponentType::Bone);
				c_bone->res_bone = rBone;
			}
		}
	}
}

void MeshLoader::LoadChannel(const aiNodeAnim * AnimNode, Channel & channel)
{
	for (uint i = 0; i < AnimNode->mNumPositionKeys; i++)
		channel.PositionKeys[AnimNode->mPositionKeys[i].mTime] = float3(AnimNode->mPositionKeys[i].mValue.x, AnimNode->mPositionKeys[i].mValue.y, AnimNode->mPositionKeys[i].mValue.z);

	for (uint i = 0; i < AnimNode->mNumRotationKeys; i++)
		channel.RotationKeys[AnimNode->mRotationKeys[i].mTime] = Quat(AnimNode->mRotationKeys[i].mValue.x, AnimNode->mRotationKeys[i].mValue.y, AnimNode->mRotationKeys[i].mValue.z, AnimNode->mRotationKeys[i].mValue.w);

	for (uint i = 0; i < AnimNode->mNumScalingKeys; i++)
		channel.ScaleKeys[AnimNode->mScalingKeys[i].mTime] = float3(AnimNode->mScalingKeys[i].mValue.x, AnimNode->mScalingKeys[i].mValue.y, AnimNode->mScalingKeys[i].mValue.z);

	channel.name = AnimNode->mNodeName.C_Str();
	App->eraseSubStr(channel.name, "_$AssimpFbx$_");
}





