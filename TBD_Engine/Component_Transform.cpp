#include "Component_Transform.h"
#include "mmgr/mmgr.h"

ComponentTransform::ComponentTransform(GameObject* GO) : Component(Component::ComponentType::Transform, GO)
{
	//transform = float4x4::FromTRS(position, rotation, scale);
	
	//transform_mat = float4x4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
}

ComponentTransform::~ComponentTransform()
{

}

// ------------GETTERS--------------

float4x4 ComponentTransform::GetTransform() const
{
	return local_transform;
}

float3 ComponentTransform::GetPosition() const
{
	return position;
}

Quat ComponentTransform::GetQuaternionRotation() const
{
	
	return rotation_quat;
}

float3 ComponentTransform::GetEulerRotation() const
{

	return rotation_euler;
}

float3 ComponentTransform::GetScale() const
{

	return scale;
}

float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_transform;
}

// ------------SETTERS--------------

void ComponentTransform::SetPosition(float3& position)
{
	this->position = position;
	UpdateLocalTransform();
}

void ComponentTransform::SetEulerRotation(float3 rot)
{
	/*float3 tmp = (rot - rotation_euler) * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(tmp.x, tmp.y, tmp.z);
	rotation_quat = rotation_quat * quaternion_rotation;
	rotation_euler = rot;*/
}

void ComponentTransform::SetScale(float3& scale)
{
	this->scale = scale;
	UpdateLocalTransform();
}

void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	local_transform = transform.Inverted() * global_transform;
	global_transform = transform;

	TransformGlobalMat(global_transform);
}

void ComponentTransform::TransformGlobalMat(const float4x4 & global)
{
	global_transform = global * local_transform;
	UpdateTRS();

	has_transformed = false;
}

void ComponentTransform::UpdateLocalTransform()
{
	local_transform = math::float4x4::FromTRS(position, rotation_quat, scale);
	has_transformed = true;
}

void ComponentTransform::UpdateTRS()
{
	local_transform.Decompose(position, rotation_quat, scale);
	rotation_euler = rotation_quat.ToEulerXYZ() * RADTODEG;
}