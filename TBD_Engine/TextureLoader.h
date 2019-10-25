#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__


#include "Globals.h"
#include "Module.h"

struct Texture 
{
	uint id = 0;
	std::string path = "NONE";
	uint width, height = 0;
};

class TextureLoader : public Module
{
public:

	TextureLoader (bool start_enabled = true);
	~TextureLoader();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Texture CreateCheckersTexture() const;
	Texture CreateDefaultTex() const;
	Texture LoadTextureFromPath(const char* path) const;
	uint CreateTexture(const void* img, uint width, uint height, int internalFormat, uint format) const;

	


public:

	Texture checkersTexture;
};

#endif //!__TEXTURE_LOADER_H__