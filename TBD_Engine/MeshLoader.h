#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include "Globals.h"
#include "Module.h"


struct MeshInfo 
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};

class MeshLoader : public Module
{
public:
	MeshLoader(bool start_enabled = true);
	~MeshLoader();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LoadFile(const char* path);

public:

	std::vector<MeshInfo*> LoadedMeshes;
};


#endif //__MESHLOADER_H__ 