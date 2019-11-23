#include "ResourceTexture.h"
#include "TextureLoader.h"
#include "Application.h"


bool ResourceTexture::LoadInMemory() 
{ 
	return App->tex_loader->LoadTextureFromLibrary(this); 
}