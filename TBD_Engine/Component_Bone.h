#ifndef __COMPONENT_BONE_H__
#define __COMPONENT_BONE_H__

#include "Component.h"
#include "ResourceBone.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class ComponentBone : public Component
{
public:
	ComponentBone(GameObject* gameObject = nullptr);
	~ComponentBone();

	void DebugDrawBones();

	float4x4 GetBoneTransform();
	float4x4 GetSkeletonTransform();
	ComponentBone* GetHipBone();

	ResourceBone* res_bone;
	ComponentType GetType() { return ComponentType::Bone; };
};

#endif //!__COMPONENT_BONE_H__