#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Globals.h"
#include "Component.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* GO);
	virtual ~ComponentMesh();

	const AABB& GetBoundingBox();

	void CleanUp();
	
//mesh
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
public:
	AABB aabb;
};

#endif