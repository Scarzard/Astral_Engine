#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__


#include "Globals.h"
#include "Module.h"

struct Texture
{
	uint id, height, width;
	std::string path;
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
	Texture LoadTextureFromPath(const char* path);
	Texture CreateDefaultTexture() const;
	uint CreateTexture(const void* img, uint width, uint height, int internalFormat, uint format) const;

	bool CreateFileDDS(const char* path) const;


public:

	Texture CheckersTexture;
	Texture DefaultTexture;


	std::vector<Texture> loaded_textures;
};

#endif //!__TEXTURE_LOADER_H__