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
	void Look(const float3 &Position, const float3 &Reference);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);


public:

	//Color background = Color(0, 0, 0, 255);

public:

	ComponentCamera* main_camera = nullptr;
};