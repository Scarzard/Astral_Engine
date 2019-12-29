#include "ResourceBone.h"
#include "Application.h"
#include "MeshLoader.h"
#include "ModuleResources.h"

#include "mmgr/mmgr.h"


bool ResourceBone::LoadInMemory() 
{ 
	return App->mesh_loader->LoadBone(this); 
}

void ResourceBone::ReleaseMemory()
{
	std::map<uint, Resource*>::const_iterator it = App->resources->resources.find(this->res_UUID);

	if (it != App->resources->resources.end())
		App->resources->resources.erase(it);

	delete[] weight;
	delete[] index_weight;

	delete this;
}