#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "TextureLoader.h"
#include "MeshLoader.h"
#include "Globals.h"

#include "Resource.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "ResourceAnimation.h"
#include "ResourceBone.h"

#include "ImGui/imgui.h"
#include "mmgr/mmgr.h"

bool ModuleResources::Init()
{
	return true;
}

bool ModuleResources::Start()
{

	mesh_icon = (ResourceTexture*)App->resources->Get(App->resources->GetNewFile("Assets/mesh_icon.jpg"));
	mesh_icon->UpdateNumReference();

	return true;
}

update_status ModuleResources::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	/*for (int i = 0; i < resources.size(); i++)
	{
		delete resources[i];
	}
	resources.clear();*/

	return true;
}

uint ModuleResources::GetNewFile(const char* file)
{
	uint ret = 0;

	// We create the correct path
	std::string path, extension; 
	App->file_system->SplitFilePath(file, nullptr, &path, &extension);
	
	if (extension == "png" || extension == "dds" || extension == "PNG" || extension == "DDS" || extension == "jpg" || extension == "tga")
	{
		path = ASSETS_TEXTURES_FOLDER + path;
	
		if (App->file_system->CopyFromOutsideFS(file, path.c_str()))
		{
			ret = ImportFile(path.c_str(), Resource::RES_TYPE::TEXTURE);
		}
	}
	else if (extension == "FBX" || extension == "fbx")
	{
		path = ASSETS_MODELS_FOLDER + path;

		if (App->file_system->CopyFromOutsideFS(file, path.c_str()))
		{
			ret = ImportFile(path.c_str(), Resource::RES_TYPE::MODEL);
		}
	}


	return ret;
}

uint ModuleResources::ImportFile(const char * new_file_in_assets, Resource::RES_TYPE type)
{
	uint ret = 0; bool import_ok = false; std::string written_file;

	ret = GetResourceInAssets(new_file_in_assets);
	if (ret == 0)//Resource does not exist
	{
		switch (type)
		{
		case Resource::RES_TYPE::TEXTURE: import_ok = App->tex_loader->LoadTextureFromPath(new_file_in_assets, written_file); break;
		}

		if (import_ok) 
		{ // If import was successful, create a new resource

			Resource* res = NewResource(type);
			res->file = new_file_in_assets;
			res->exported_file = written_file;
			ret = res->res_UUID;
		}
	}

	return ret;
}

Resource* ModuleResources::NewResource(Resource::RES_TYPE type)
{
	Resource* ret = nullptr;
	uint uid = App->GetRandomUUID();

	switch (type) 
	{
	case Resource::RES_TYPE::TEXTURE: 
		ret = (Resource*) new ResourceTexture(uid); 
		if (ret != nullptr)
			tex_resources[uid] = (ResourceTexture*)ret;
		break;
	case Resource::RES_TYPE::MESH:
		ret = (Resource*) new ResourceMesh(uid);
		break;
	case Resource::RES_TYPE::ANIMATION:
		ret = (Resource*) new ResourceAnimation(uid);
		break;
	case Resource::RES_TYPE::BONE:
		ret = (Resource*) new ResourceBone(uid);
		break;

	}

	if (ret != nullptr)
		resources[uid] = ret;
	return ret;
}

uint ModuleResources::GetResourceInAssets(const char* path)const
{
	
	for (std::map<uint, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		std::string s = it->second->file;
		if (s.compare(path) == 0)
			return it->first;
	}

	return 0;
}

uint ModuleResources::IsResourceInLibrary(const char* name) const
{
	for (std::map<uint, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		char n[250];
		sprintf_s(n, 250, "%s", name);
		App->file_system->NormalizePath(n);

		std::string s = App->GetNameFromPath(it->second->exported_file);

		if (s.compare(n) == 0)
			return it->first;
	}

	return 0;
}

Resource* ModuleResources::Get(uint uid) 
{
	std::map<uint, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
		return it->second;

	return nullptr;
}

void ModuleResources::DrawExplorer()
{
	int i = 0;

	for (std::map<uint, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->second != nullptr && it->second->loaded > 0)
		{
			
			if (it->second->type == Resource::RES_TYPE::TEXTURE)
			{
				i++;
				std::map<uint, ResourceTexture*>::const_iterator tex = tex_resources.find(it->first);
				ImGui::ImageButton((ImTextureID*)tex_resources[it->first]->texture, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Name: %s\nUUID: %u\nReferences: %u", App->GetNameFromPath(it->second->file).c_str(), it->second->res_UUID, it->second->GetNumReferences());
				
					
			}

			if (it->second->type == Resource::RES_TYPE::MESH)
			{
				i++;
				ImGui::ImageButton((ImTextureID*)mesh_icon->texture, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Name: %s\nUUID: %u\nReferences: %u", App->GetNameFromPath(it->second->exported_file).c_str(), it->second->res_UUID, it->second->GetNumReferences());


			}

		}



		if (i < 7)
		{
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(25.0f, 10.0f));
			ImGui::SameLine();

		}
		else
		{
			i = 0;
			ImGui::Dummy(ImVec2(875.0f, 10.0f));
		}
	}
}
