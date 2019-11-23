#ifndef _COMPONENT_CAMERA_H
#define _COMPONENT_CAMERA_H

#include "Component.h"
#include "Globals.h"
#include "glmath.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentCamera : public Component
{
public:
	ALIGN_CLASS_TO_16
	ComponentCamera(GameObject* gameObject =  nullptr);
	~ComponentCamera();

	void Update();
	void LookAt(const float3& position);

	//---------Getters------------
	float GetFOV() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetAspectRatio() const;

	float4x4 GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;
	float4x4 GetOpenGLView() const;
	float4x4 GetOpenGLProjection() const;

	//---------Setters------------
	void SetFOV(float fov);
	void SetAspectRatio(float aspect);
	void SetNearPlane(float near_plane);
	void SetFarPlane(float far_plane);
	void UpdateTransform();

	void UpdatePlanes();

	void DrawFrustum();
	
	bool ContainsAABB(const AABB& reference);
	static bool ContainsAABB(const Frustum& frustum, const AABB& reference);

public:
	Frustum frustum;
	bool frustum_view = true;
	bool has_transformed = false;
	bool culling = false;

private:
	float aspect_ratio = 0.0f;

	Plane planes[6];
	
};
#endif // !_COMPONENT_CAMERA_H
