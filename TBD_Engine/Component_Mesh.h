#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Globals.h"
#include "Component.h"
#include "ResourceMesh.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* GO);
	virtual ~ComponentMesh();

	const AABB& GetBoundingBox();
	void UpdateAABB();
	void UpdateGlobalAABB();
	void DrawAABB();

	void CleanUp();

	// Load & Save 
	//void Load(uint obj_num, const nlohmann::json &scene_file);
	void Save(uint obj_num, nlohmann::json &scene_file);
	
//mesh
public:
	//bounding box
	AABB aabb;
	AABB global_aabb;

	bool draw_normals = false;
	ResourceMesh* res_mesh = nullptr;
};

#endif