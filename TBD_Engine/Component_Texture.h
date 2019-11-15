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

public:
	
	bool Checers_texture;

	Texture texture;
};

#endif