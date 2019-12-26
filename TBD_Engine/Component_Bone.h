#ifndef __COMPONENT_BONE_H__
#define __COMPONENT_BONE_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class ComponentBone : public Component
{
public:
	ComponentBone(GameObject* gameObject = nullptr);
	~ComponentBone();

	float4x4 GetBoneTransform();
	ComponentBone* GetParentBone();

	ComponentType GetType() { return ComponentType::Bone; };
};

#endif //!__COMPONENT_BONE_H__