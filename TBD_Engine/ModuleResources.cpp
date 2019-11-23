#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "TextureLoader.h"
#include "Globals.h"

#include "Resource.h"
#include "ResourceTexture.h"

#include "mmgr/mmgr.h"

bool ModuleResources::Init()
{
	return true;
}

bool ModuleResources::Start()
{
	return true;
}

update_status ModuleResources::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
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

	return ret;
}

uint ModuleResources::ImportFile(const char * new_file_in_assets, Resource::RES_TYPE type)
{
	uint ret = 0; bool import_ok = false; std::string written_file;

	ret = ResourceInAssets(new_file_in_assets);
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
	case Resource::RES_TYPE::TEXTURE: ret = (Resource*) new ResourceTexture(uid); break;

	}

	if (ret != nullptr)
		resources[uid] = ret;
	return ret;
}

uint ModuleResources::ResourceInAssets(const char* path)const
{
	
	for (std::map<uint, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		std::string s = it->second->file;
		if (s.compare(path) == 0)
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
