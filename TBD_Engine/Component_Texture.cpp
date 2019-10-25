#include "Component_Texture.h"

#include "glew/include/GL/glew.h"

#include "mmgr/mmgr.h"

ComponentTexture::ComponentTexture(GameObject* GO) : Component(Component::ComponentType::Mesh, GO)
{
	texture.id = 0;
	texture.height = 0;
	texture.width = 0;
	texture.path = "No path";
}

ComponentTexture::~ComponentTexture()
{

}

void ComponentTexture::CleanUp()
{
	
}