#ifndef __COMPONENT_TRANSF_H__
#define __COMPONENT_TRANSF_H__

#include "Globals.h"
#include "Component.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ALIGN_CLASS_TO_16

	ComponentTransform(GameObject* GO);

	virtual ~ComponentTransform();

	//Transform getters
	float3 GetPosition() const;
	Quat GetQuaternionRotation() const;
	float3 GetEulerRotation() const;
	float3 GetScale() const;
	float4x4 GetTransform() const;
	float4x4 GetGlobalTransform() const;

	//Transform setters
	void SetPosition(float3& position);
	void SetEulerRotation(float3 rot);
	void SetScale(float3& scale);
	void SetGlobalTransform(float4x4 transform);
	void TransformGlobalMat(const float4x4& global);
	void ResetTransform();
	void ResetPosition();

	void UpdateLocalTransform();
	void UpdateTRS();

public:
	float4x4 local_matrix = float4x4::identity;
	float4x4 global_matrix = float4x4::identity;

	float3 position = float3::zero;
	Quat rotation_quat = Quat::identity;
	float3 rotation_euler = float3::zero;
	float3 scale = float3::one;

	bool has_transformed = false;
};

#endif //__COMPONENT_TRANSF_H__