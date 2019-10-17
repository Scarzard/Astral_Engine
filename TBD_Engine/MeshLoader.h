#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include "Globals.h"
#include "Module.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"


struct MeshInfo 
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float3* vertex = nullptr;

	uint id_tex_coords = 0; 
	uint num_tex_coords = 0;
	float* tex_coords = nullptr;
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