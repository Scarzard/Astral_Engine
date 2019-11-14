#include "Component_Texture.h"
#include "Application.h"

#include "glew/include/GL/glew.h"

#include "mmgr/mmgr.h"

ComponentTexture::ComponentTexture(GameObject* GO) : Component(Component::ComponentType::Texture, GO)
{
	
}

ComponentTexture::~ComponentTexture()
{

}

void ComponentTexture::CleanUp()
{
	
}