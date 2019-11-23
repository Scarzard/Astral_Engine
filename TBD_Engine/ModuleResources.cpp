#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include <string>

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

Resource * ModuleResources::NewResource(Resource::RESOURCE_TYPE type)
{
	Resource* ret = nullptr;
	uint uid = GenerateNewUID();

	switch (type) 
	{
	case Resource::RESOURCE_TYPE::MESH:

	}

	if (ret != nullptr)
		resources[uid] = ret;
	return ret;
}

Resource * ModuleResources::Get(uint uid)
{
	std::map<uint, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
		return it->second;
	return nullptr;
}
