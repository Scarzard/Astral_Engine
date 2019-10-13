#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__


#include "Globals.h"
#include "Module.h"


class TextureLoader : public Module
{
public:

	TextureLoader (bool start_enabled = true);
	~TextureLoader();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	uint LoadCheckImage() const;
	uint CreateTextureFromFile(const char* path) const;
	uint CreateCheckersTexture(uint width, uint height) const;
	uint GetCheckerTextureID() const;


public:

	uint id_checkersTexture = 0;
};

#endif //!__TEXTURE_LOADER_H__