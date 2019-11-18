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

	ComponentCamera(GameObject* gameObject);
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

public:
	math::Frustum frustum;
	bool frustrum_view = true;

private:

	float4x4 view_mat;
	float4x4 projection_mat;
	float aspect_ratio = 0.0f;
	bool culling = false;
};
#endif // !_COMPONENT_CAMERA_H
