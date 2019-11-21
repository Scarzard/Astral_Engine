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
	void Move(const float3 &movement);
	void LookAt(const float3 &spot, float dist = 0.0f);
	void CenterToObject(GameObject* obj);

	//Getters
	float3 GetPosition() const;
	float* GetOpenGLView() const;
	float* GetOpenGLProjection() const;
	bool* GetProjectionUpdateFlag() const;
	ComponentCamera* GetActiveCamera() const;
	const Frustum& GetActiveFrustum() const;

	bool Intersects(const AABB& refBox) const;

private:

	// Input Checks
	void MoveCamera(float& movSpeed);
	void DragCamera(float delta_x, float delta_y);
	void Zoom(float delta_z);
	bool FirstPersonCamera(float& movSpeed);

	// Camera Rotations
	void Orbit(float motion_x, float motion_y);
	void Rotate(float motion_x, float motion_y);

public:
	float3 looking_at;
	GameObject* obj_camera = nullptr;
	ComponentCamera* main_camera = nullptr;
	ComponentCamera* active_camera = nullptr;
	bool looking = false;
};