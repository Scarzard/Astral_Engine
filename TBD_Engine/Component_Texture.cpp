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

void ComponentTexture::Save(uint obj_num, nlohmann::json &scene)
{
	scene["Game Objects"][obj_num]["Components"]["Texture"]["UUID"] = UUID;
	scene["Game Objects"][obj_num]["Components"]["Texture"]["Checkers"] = Checers_texture;
	scene["Game Objects"][obj_num]["Components"]["Texture"]["Path"] = texture.path; // temporal - TODO: resourceID
}

void ComponentTexture::CleanUp()
{
	
}