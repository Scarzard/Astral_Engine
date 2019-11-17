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

	void CameraControl();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);

	float* GetViewMatrix();

	void CalculateViewMatrix();

public:
	vec3 X, Y, Z, Position, Reference;

public:

	bool perspective = true;

	float z_near = 1.0f;
	float z_far = 100.0f;

	float fov_y = 60.0f;

	float width = 1280.0f;
	float height = 720.0f;

	Frustum frustum;
private:

	mat4x4 ViewMatrix, ProjectionMatrix;

	float3 frustrum_edges[12];

};
#endif // !_COMPONENT_CAMERA_H
