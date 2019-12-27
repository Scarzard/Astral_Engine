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
	scene[my_GO->name]["Components"]["Texture"]["UUID"] = std::to_string(UUID);
	scene[my_GO->name]["Components"]["Texture"]["Checkers"] = std::to_string(Checers_texture);
	if(res_texture!=nullptr)
		scene[my_GO->name]["Components"]["Texture"]["ResourceName"] = res_texture->exported_file; // temporal - TODO: resourceID
}

void ComponentTexture::CleanUp()
{
	if (res_texture != nullptr)
	{
		res_texture->loaded -= 1;
		if (res_texture->loaded == 0)
			res_texture->ReleaseMemory();
	}
}