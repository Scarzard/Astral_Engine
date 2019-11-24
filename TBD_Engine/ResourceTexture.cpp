#include "ResourceTexture.h"
#include "TextureLoader.h"
#include "ModuleResources.h"
#include "Application.h"

bool ResourceTexture::LoadInMemory() 
{ 
	return App->tex_loader->LoadTextureFromLibrary(this); 
}

void ResourceTexture::ReleaseMemory()
{
	std::map<uint, Resource*>::const_iterator it = App->resources->resources.find(this->res_UUID);
	std::map<uint, ResourceTexture*>::const_iterator tex = App->resources->tex_resources.find(this->res_UUID);
	App->resources->resources.erase(it);
	App->resources->tex_resources.erase(tex);
	delete this;
}