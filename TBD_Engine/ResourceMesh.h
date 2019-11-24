#ifndef __ResourceMesh_H__
#define __ResourceMesh_H__

#include "Resource.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint uuid) : Resource(uuid, RES_TYPE::MESH) {}

	virtual ~ResourceMesh() {}

	//void Save(const char* resourceNum, nlohmann::json &config) const;
	//void Load(const char* resourceNum, const nlohmann::json &config);

	bool LoadInMemory();

public:
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float3* vertex = nullptr;

	uint id_tex_coords = 0;
	uint num_tex_coords = 0;
	float* tex_coords = nullptr;

	uint num_normals = 0;
	float3* face_center = nullptr;
	float3* face_normal = nullptr;

	bool draw_normals = false;

	//bounding box
	AABB aabb;
	AABB global_aabb;
};

#endif // __ResourceMesh_H__