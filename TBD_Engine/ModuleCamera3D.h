#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Component_Camera.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//Moving camera

	float3 GetPosition() const;
	void Look(const float3& position);
	void CenterOn(const float3& position, float distance);

private:

	void Move(float dt);
	void Orbit(float motion_x, float motion_y);
	void LookAt(float motion_x, float motion_y);
	void Zoom(float zoom);

public:
	float3 looking_at;
	ComponentCamera* main_camera = nullptr;
	bool looking = false;
};