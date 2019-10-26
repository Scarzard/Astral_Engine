#include "Component_Texture.h"
#include "Application.h"

#include "glew/include/GL/glew.h"

#include "mmgr/mmgr.h"

ComponentTexture::ComponentTexture(GameObject* GO) : Component(Component::ComponentType::Mesh, GO)
{
	this->texture = App->tex_loader->DefaultTexture;
	this->Checers_texture = false;
}

ComponentTexture::~ComponentTexture()
{

}

void ComponentTexture::CleanUp()
{
	
}