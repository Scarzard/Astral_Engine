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

		//Child of root node
		App->scene_intro->root->SetChild(Empty);

		if (root_node->mNumChildren > 0)
			for (int i = 0; i < root_node->mNumChildren; ++i)
			{
				LoadNode(scene, root_node->mChildren[i], Empty, full_path, output_file, mesh_collector, go_collector);
			}


		LoadBones(mesh_collector, go_collector, Empty, full_path);

		if (scene->HasAnimations() == true)
		{
			Empty->CreateComponent(Component::ComponentType::Animation);

			for (int i = 0; i < scene->mNumAnimations; i++)
			{
				std::string str = App->GetNameFromPath(full_path);
				if (i > 0)
					str.append("_%i", i);

				if (App->resources->IsResourceInLibrary(str.c_str(), Resource::RES_TYPE::ANIMATION) != 0)
				{
					Empty->GetComponentAnimation()->res_anim = (ResourceAnimation*)App->resources->Get(App->resources->IsResourceInLibrary(str.c_str(), Resource::RES_TYPE::ANIMATION));
				}
				else
				{
					ResourceAnimation* anim = (ResourceAnimation*)App->resources->NewResource(Resource::RES_TYPE::ANIMATION);
					anim->name = str;
					ExportAnim(anim->exported_file, str, scene->mAnimations[i]->mDuration, scene->mAnimations[i]->mTicksPerSecond, scene->mAnimations[i]->mNumChannels, scene->mAnimations[i]->mChannels);
					anim->file = full_path;
										
					Empty->GetComponentAnimation()->res_anim = anim;
				}

				if (Empty->GetComponentAnimation()->res_anim != nullptr)
					Empty->GetComponentAnimation()->res_anim->UpdateNumReference();
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

		if (new_mesh->HasBones())
		{
			mesh_collect.push_back(new_mesh);
			go_collect.push_back(child);
		}

		//If this mesh is inmemory 
		if (App->resources->IsResourceInLibrary(child->name.c_str(), Resource::RES_TYPE::MESH) != 0)
		{
			mesh->res_mesh = (ResourceMesh*)App->resources->Get(App->resources->IsResourceInLibrary(child->name.c_str(), Resource::RES_TYPE::MESH));
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------- ANIMATION FUNCTIONS ----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


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
	res_bone->matrix = float4x4(bone->mOffsetMatrix.a1, bone->mOffsetMatrix.a2, bone->mOffsetMatrix.a3, bone->mOffsetMatrix.a4,
		bone->mOffsetMatrix.b1, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.b4,
		bone->mOffsetMatrix.c1, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.c4,
		bone->mOffsetMatrix.d1, bone->mOffsetMatrix.d2, bone->mOffsetMatrix.d3, bone->mOffsetMatrix.d4);

	//-------

	res_bone->weight = new float[res_bone->NumWeights];
	res_bone->index_weight = new uint[res_bone->NumWeights];

	for (int i = 0; i < res_bone->NumWeights; i++)
	{
		res_bone->weight[i] = bone->mWeights[i].mWeight;
		res_bone->index_weight[i] = bone->mWeights[i].mVertexId;
	}

	

}

void MeshLoader::LoadBones(std::vector<aiMesh*> mesh_collect, std::vector<GameObject*> go_collect, GameObject* root, const char* full_path)
{
	std::map<std::string, GameObject*> go_map;
	FillMap(go_map, root);

	for (int i = 0; i < mesh_collect.size(); i++)
	{
		for (int j = 0; j < mesh_collect[i]->mNumBones; j++)
		{
			ResourceBone* rBone = nullptr;

			if (App->resources->IsResourceInLibrary(mesh_collect[i]->mBones[j]->mName.C_Str(), Resource::RES_TYPE::BONE) != 0)
			{
				rBone = (ResourceBone*)App->resources->IsResourceInLibrary(mesh_collect[i]->mBones[j]->mName.C_Str(), Resource::RES_TYPE::BONE);
			}
			else
			{
				rBone = (ResourceBone*)App->resources->NewResource(Resource::BONE);

				ResourceBone* tmp = new ResourceBone(App->GetRandomUUID());

				LoadBoneData(mesh_collect[i]->mBones[j], rBone, go_collect[i]->GetComponentMesh()->res_mesh->GetUUID()); //temporary 

				ExportBone(rBone->exported_file, tmp, mesh_collect[i]->mBones[j], go_collect[i]->GetComponentMesh()->res_mesh->GetUUID());
				rBone->file = full_path;
			}

			std::map<std::string, GameObject*>::iterator bone = go_map.find(mesh_collect[i]->mBones[j]->mName.C_Str());
			if (bone != go_map.end())
			{
				ComponentBone* c_bone = (ComponentBone*)bone->second->CreateComponent(Component::ComponentType::Bone);
				c_bone->res_bone = rBone;
				// if(rBone)rBone->UpdateNumReference();
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

bool MeshLoader::ExportAnim(std::string & output_file, std::string name, float duration, float ticksPerSecond, uint numChannels, aiNodeAnim** mChannels)
{
	bool ret = false;

	//----------------------------- CALCULATE SIZE ----------------------------------

	//Animation Duration, TicksperSec, numChannels
	uint size = sizeof(float) + sizeof(float) + sizeof(uint);

	// Size of Channels
	Channel* channels = new Channel[numChannels];
	for (int i = 0; i < numChannels; i++)
	{
		LoadChannel(mChannels[i], channels[i]);

		//Size name + Name
		size+= sizeof(uint) + channels[i].name.size() + sizeof(uint) * 3;

		//PositionsKeys size
		size += sizeof(double) * channels[i].PositionKeys.size();
		size += sizeof(float)* 3 * channels[i].PositionKeys.size();
		//RotationsKeys size
		size += sizeof(double) * channels[i].RotationKeys.size();
		size += sizeof(float)* 4 * channels[i].RotationKeys.size();
		//ScalesKeys size
		size += sizeof(double) * channels[i].ScaleKeys.size();
		size += sizeof(float)* 3 * channels[i].ScaleKeys.size();
	}
	//-------------------------------------------------------------------------------

	//---------------------------------- Allocate -----------------------------------
	char* data = new char[size]; 
	char* cursor = data;

	//Duration
	memcpy(cursor, &duration, sizeof(float));
	cursor += sizeof(float);

	//TicksperSec
	memcpy(cursor, &ticksPerSecond, sizeof(float));
	cursor += sizeof(float);

	//numChannels
	memcpy(cursor, &numChannels, sizeof(uint));
	cursor += sizeof(uint);

	//Channels
	for (int i = 0; i < numChannels; i++)
	{
		//Name size 
		uint SizeofName = channels[i].name.size();
		memcpy(cursor, &SizeofName, sizeof(uint));
		cursor += sizeof(uint);

		//name data
		memcpy(cursor, channels[i].name.c_str(), channels[i].name.size());
		cursor += channels[i].name.size();

		//Poskeys, Rotkeys and Scalekeys SIZES
		uint ranges[3] = { channels[i].PositionKeys.size(), channels[i].RotationKeys.size(), channels[i].ScaleKeys.size() };
		memcpy(cursor, ranges, sizeof(ranges));
		cursor += sizeof(ranges);

		//PositionKeys
		std::map<double, float3>::const_iterator it = channels[i].PositionKeys.begin();
		for (it = channels[i].PositionKeys.begin(); it != channels[i].PositionKeys.end(); it++)
		{
			memcpy(cursor, &it->first, sizeof(double));
			cursor += sizeof(double);

			memcpy(cursor, &it->second, sizeof(float) * 3);
			cursor += sizeof(float) * 3;
		}

		//RotationKeys
		std::map<double, Quat>::const_iterator it2 = channels[i].RotationKeys.begin();
		for (it2 = channels[i].RotationKeys.begin(); it2 != channels[i].RotationKeys.end(); it2++)
		{
			memcpy(cursor, &it2->first, sizeof(double));
			cursor += sizeof(double);

			memcpy(cursor, &it2->second, sizeof(float) * 4);
			cursor += sizeof(float) * 4;
		}

		//ScaleKeys
		std::map<double, float3>::const_iterator it3 = channels[i].ScaleKeys.begin();
		for (it3 = channels[i].ScaleKeys.begin(); it3 != channels[i].ScaleKeys.end(); it3++)
		{
			memcpy(cursor, &it3->first, sizeof(double));
			cursor += sizeof(double);

			memcpy(cursor, &it3->second, sizeof(float) * 3);
			cursor += sizeof(float) * 3;
		}
	}

	ret = App->file_system->SaveUnique(output_file, data, size, LIBRARY_ANIMS_FOLDER, name.c_str(), "anim");

	if (!ret)
		App->LogInConsole("Failed exporting %s.anim into Library/animations floder", name.c_str());


	RELEASE_ARRAY(channels);
	
	if (data)
	{
		delete[] data;
		data = nullptr;
		cursor = nullptr;
	}

	return ret;
}

bool MeshLoader::LoadAnim(ResourceAnimation* anim)
{
	bool ret = true;

	char* buffer = nullptr;
	App->file_system->Load(anim->exported_file.c_str(), &buffer); //put data file in buffer

	if (buffer)
	{
		char* cursor = buffer;
		uint bytes = 0;

		//Duration
		memcpy(&anim->duration, cursor, sizeof(float));
		cursor += sizeof(float);

		//TicksperSec
		memcpy(&anim->ticksPerSecond, cursor, sizeof(float));
		cursor += sizeof(float);

		//numChannels
		memcpy(&anim->numChannels, cursor, sizeof(uint));
		cursor += sizeof(uint);

		anim->channels = new Channel[anim->numChannels];
		for (uint i = 0; i < anim->numChannels; i++)
		{
			bytes = 0;
			// Load channel name -----------------------
			uint SizeofName = 0;
			memcpy(&SizeofName, cursor, sizeof(uint));
			cursor += sizeof(uint);

			char* string = new char[SizeofName + 1];
			bytes = sizeof(char) * SizeofName;
			memcpy(string, cursor, bytes);
			cursor += bytes;

			string[SizeofName] = '\0';
			anim->channels[i].name = string;
			RELEASE_ARRAY(string);
			//-------------------------------------------

			//Poskeys, Rotkeys and Scalekeys SIZES
			uint ranges[3];
			memcpy(&ranges, cursor, sizeof(uint)*3);
			cursor += sizeof(uint)*3;

			//Load PosKeys
			for (int j = 0; j < ranges[0]; j++)
			{
				double key;
				memcpy(&key, cursor, sizeof(double));
				cursor += sizeof(double);

				float pos[3];
				memcpy(&pos, cursor, sizeof(float) * 3);
				cursor += sizeof(float) * 3;

				anim->channels[i].PositionKeys[key] = (float3)pos;
			}

			//Load RotKeys
			for (int j = 0; j < ranges[1]; j++)
			{
				double key;
				memcpy(&key, cursor, sizeof(double));
				cursor += sizeof(double);

				float rot[4];
				memcpy(&rot, cursor, sizeof(float) * 4);
				cursor += sizeof(float) * 4;

				anim->channels[i].RotationKeys[key] = (Quat)rot;
			}

			//Load ScaleKeys
			for (int j = 0; j < ranges[2]; j++)
			{
				double key;
				memcpy(&key, cursor, sizeof(double));
				cursor += sizeof(double);

				float scale[3];
				memcpy(&scale, cursor, sizeof(float) * 3);
				cursor += sizeof(float) * 3;

				anim->channels[i].ScaleKeys[key] = (float3)scale;
			}

		}

		RELEASE_ARRAY(buffer);
		cursor = nullptr;

		ret = true;

	}

	return ret;
}

bool MeshLoader::ExportBone(std::string & output_file, ResourceBone* tmp, aiBone* mBones, uint id)
{
	bool ret = false;

	LoadBoneData(mBones, tmp, id);

	//----------------------------- CALCULATE SIZE ------------------------------------------------------------------------------------

	//Bone meshID, numWeights, matrix, weights, index_weigths
	uint size = sizeof(uint) + sizeof(uint) + sizeof(float) * 16 + tmp->NumWeights * sizeof(float) + tmp->NumWeights * sizeof(uint);

	//---------------------------------------------------------------------------------------------------------------------------------
	//------------------------------- Allocate ---------------------------------------------------------------------------------------
	
	char* data = new char[size];
	char* cursor = data;

	// meshID
	memcpy(cursor, &tmp->meshID, sizeof(uint));
	cursor += sizeof(uint);

	// numWeights
	memcpy(cursor, &tmp->NumWeights, sizeof(uint));
	cursor += sizeof(uint);

	// matrix
	memcpy(cursor, &tmp->matrix, sizeof(float) * 16);
	cursor += sizeof(float) * 16;

	//Weights
	memcpy(cursor, tmp->weight, sizeof(float) * tmp->NumWeights);
	cursor += sizeof(float) * tmp->NumWeights;

	//index_weights
	memcpy(cursor, tmp->index_weight, sizeof(uint) * tmp->NumWeights);
	cursor += sizeof(uint) * tmp->NumWeights;

	ret = App->file_system->SaveUnique(output_file, data, size, LIBRARY_BONES_FOLDER, mBones->mName.C_Str(), "bone");

	if (!ret)
		App->LogInConsole("Failed exporting %s.anim into Library/animations floder", mBones->mName.C_Str());

	if (data)
	{
		delete[] data;
		data = nullptr;
		cursor = nullptr;
	}

	if (tmp->weight)
	{
		delete[] tmp->weight;
		tmp->weight = nullptr;
	}
	
	if (tmp->index_weight)
	{
		delete[] tmp->index_weight;
		tmp->index_weight = nullptr;
	}

	delete tmp;

	return ret;
}





