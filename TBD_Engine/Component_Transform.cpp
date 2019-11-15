#include "Component_Transform.h"
#include "GameObject.h"
#include "mmgr/mmgr.h"

ComponentTransform::ComponentTransform(GameObject* GO) : Component(Component::ComponentType::Transform, GO)
{
	local_matrix = float4x4::FromTRS(position, rotation_quat, scale);
}

ComponentTransform::~ComponentTransform()
{

}

// ------------GETTERS--------------

float4x4 ComponentTransform::GetTransform() const
{

	return local_matrix;
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
	return global_matrix;
}

// ------------SETTERS--------------

void ComponentTransform::SetPosition(float3& position)
{
	this->position = position;
	UpdateLocalTransform();
}

void ComponentTransform::SetEulerRotation(float3 rot)
{
	float3 tmp = (rot - rotation_euler) * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(tmp.x, tmp.y, tmp.z);
	rotation_quat = rotation_quat * quaternion_rotation;
	rotation_euler = rot;
	UpdateLocalTransform();
}

void ComponentTransform::SetScale(float3& scale)
{
	if (scale.x > 0.0f && scale.y > 0.0f && scale.z > 0.0f)
		this->scale = scale;

	UpdateLocalTransform();
}

void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	local_matrix = transform.Inverted() * global_matrix;
	global_matrix = transform;

	TransformGlobalMat(global_matrix);
}

void ComponentTransform::TransformGlobalMat(const float4x4 & global)
{
	global_matrix = global * local_matrix;
	UpdateTRS();

	has_transformed = false;
}

void ComponentTransform::UpdateLocalTransform()
{
	local_matrix = math::float4x4::FromTRS(position, rotation_quat, scale);
	has_transformed = true;
}

void ComponentTransform::UpdateTRS()
{
	local_matrix.Decompose(position, rotation_quat, scale);
	rotation_euler = rotation_quat.ToEulerXYZ() * RADTODEG;
}

