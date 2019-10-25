#ifndef __COMPONENT_TEXTURE_H__
#define __COMPONENT_TEXTURE_H__

#include "Globals.h"
#include "Component.h"



class ComponentTexture : public Component
{
public:

	ComponentTexture(GameObject* GO);
	virtual ~ComponentTexture();


	void CleanUp();

public:
	
	uint Texture = 0;
	std::string texture_path = "none";
	bool Checers_texture = false;
};

#endif