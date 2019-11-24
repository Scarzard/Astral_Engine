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

void ComponentTransform::Save(uint obj_num, nlohmann::json &scene)
{
	scene[my_GO->name]["Components"]["Transform"]["UUID"] = std::to_string(UUID);
	scene[my_GO->name]["Components"]["Transform"]["PositionX"] = std::to_string(position.x);
	scene[my_GO->name]["Components"]["Transform"]["PositionY"] = std::to_string(position.y);
	scene[my_GO->name]["Components"]["Transform"]["PositionZ"] = std::to_string(position.z);
	scene[my_GO->name]["Components"]["Transform"]["RotationX"] = std::to_string(rotation_euler.x);
	scene[my_GO->name]["Components"]["Transform"]["RotationY"] = std::to_string(rotation_euler.y);
	scene[my_GO->name]["Components"]["Transform"]["RotationZ"] = std::to_string(rotation_euler.z);
	scene[my_GO->name]["Components"]["Transform"]["ScaleX"] = std::to_string(scale.x);
	scene[my_GO->name]["Components"]["Transform"]["ScaleY"] = std::to_string(scale.y);
	scene[my_GO->name]["Components"]["Transform"]["ScaleZ"] = std::to_string(scale.z);
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

void ComponentTransform::ResetTransform()
{
	local_matrix = math::float4x4::FromTRS(position, rotation_quat, scale);
	rotation_euler = rotation_quat.ToEulerXYZ() * RADTODEG;

	has_transformed = true;
}

void ComponentTransform::ResetPosition()
{
	this->position = float3::zero;
	this->scale = float3::one;
	this->rotation_quat = Quat::identity;
	
	ResetTransform();
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

