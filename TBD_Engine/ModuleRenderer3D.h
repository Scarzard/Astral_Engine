#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include "MathGeoLib/include/MathGeoLib.h"

#define MAX_LIGHTS 8
class GameObject;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void NewVertexBuffer(float3* vertex, uint &size, uint &id_vertex);
	void NewIndexBuffer(uint* index, uint &size, uint &id_index);
	void NewTexBuffer(float* tex_coords, uint &num_tex_coords, uint &id_tex_coords);
	void Draw(GameObject* m) const;


public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

		
};