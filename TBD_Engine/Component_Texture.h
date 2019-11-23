#ifndef __COMPONENT_TEXTURE_H__
#define __COMPONENT_TEXTURE_H__

#include "Globals.h"
#include "Component.h"
#include "TextureLoader.h"

class ComponentTexture : public Component
{
public:

	ComponentTexture(GameObject* GO);
	virtual ~ComponentTexture();


	void CleanUp();

	// Load & Save 
	//void Load(uint obj_num, const nlohmann::json &scene_file);
	void Save(uint obj_num, nlohmann::json &scene_file);

public:
	
	bool Checers_texture = false;

	Texture texture;
};

#endif