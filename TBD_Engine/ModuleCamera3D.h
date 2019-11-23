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

	//Mouse picking
	const LineSegment* GetLastRay()const { return &hit; }

	bool Intersects(const AABB& refBox) const;
	void MouseClick();
	void OnClick(const vec2& normMousePos);

private:

	// Input Checks
	void MoveCamera(float& movSpeed);
	void DragCamera(float delta_x, float delta_y);
	void Zoom(float delta_z);
	bool FirstPersonCamera(float& movSpeed);

	Color background = Color(0.1f, 0.1f, 0.1f, 255);


	// Camera Rotations
	void Orbit(float motion_x, float motion_y);
	void Rotate(float motion_x, float motion_y);
	void DrawRay();
public:
	LineSegment hit;
	float3 looking_at;
	GameObject* obj_camera = nullptr;
	ComponentCamera* main_camera = nullptr;
	ComponentCamera* active_camera = nullptr;
	bool looking = false;
};