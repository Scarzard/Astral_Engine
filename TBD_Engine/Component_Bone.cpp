#include "Component_Bone.h"
#include "GameObject.h"

ComponentBone::ComponentBone(GameObject * gameObject) : Component(Component::ComponentType::Bone, gameObject)
{
}

ComponentBone::~ComponentBone()
{
}

float4x4 ComponentBone::GetBoneTransform()
{
	float4x4 tmp = my_GO->GetComponentTransform()->GetGlobalTransform();
	return GetParentBone()->my_GO->parent->parent->GetComponentTransform()->GetGlobalTransform().Inverted() * tmp;
}

ComponentBone* ComponentBone::GetParentBone()
{
	ComponentBone* pBone = my_GO->parent->GetComponentBone();

	if (pBone == nullptr)
	{
		return this;
	}
	else
		return pBone->GetParentBone();
}
