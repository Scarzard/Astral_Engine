#ifndef __COMPONENT_TRANSF_H__
#define __COMPONENT_TRANSF_H__

#include "Globals.h"
#include "Component.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component
{
public:

	ComponentTransform(GameObject* GO, float3 position = float3::zero, Quat rotation = Quat::identity, float3 scale = float3::one);
	~ComponentTransform();

	void CleanUp();

	//Transform getters
	float4x4 GetTransform() const;
	float3 GetPosition() const;
	Quat GetQuaternionRotation() const;
	float3 GetEulerRotation() const;
	float3 GetScale() const;

	//Transform setters
	void SetPosition(float3& position);
	void SetQuaternionRotation(Quat& rotation);
	void SetEulerRotation(float3 rot);
	void SetScale(float3& scale);

	void SetComponent(float3& position,  float3& scale,  Quat& rotation);

public:
	float4x4 transform = float4x4::zero;
	
	float3 position = float3::zero;
	Quat rotation_quat = Quat::identity;
	float3 rotation_euler = float3::zero;
	float3 scale = float3::one;
};

#endif //__COMPONENT_TRANSF_H__