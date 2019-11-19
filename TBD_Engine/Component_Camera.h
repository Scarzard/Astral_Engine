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

	//---------Getters------------
	float GetFOV() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetAspectRatio() const;
	float* GetViewMatrix();
	float* GetProjectionMatrix();

	//---------Setters------------
	void SetFOV(float fov);
	void SetAspectRatio(float aspect);
	void SetNearPlane(float near_plane);
	void SetFarPlane(float far_plane);

	void DrawFrustum();
	void OnUpdateTransform(const float4x4& global, const float4x4& parent_global = float4x4::identity);

private:
	void UpdateMatrixView();

public:
	math::Frustum frustum;
	bool frustum_view = false;
	bool has_transformed = false;
	Plane		planes[6];

private:

	float4x4 view_mat;
	float4x4 projection_mat;
	float aspect_ratio = 0.0f;
	bool culling = false;
};
#endif // !_COMPONENT_CAMERA_H
