#include "Application.h"
#include "Component_Transform.h"
#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "mmgr/mmgr.h"

ComponentTransform::ComponentTransform(GameObject* GO, float3 position, Quat rotation, float3 scale) : Component(Component::ComponentType::Transform, GO)
{
	//transform = float4x4::FromTRS(position, rotation, scale);
}

ComponentTransform::~ComponentTransform()
{

}

bool ComponentTransform::Update()
{
	//Update transf
	//UpdateTranformations();
	return true;
}

void ComponentTransform::CleanUp()
{

}

void ComponentTransform::UpdateTranformations()
{
	GameObject* go = App->gui->ins_window->selected_GO;
	if (has_transformed)
	{
		transform_mat = float4x4::FromTRS(position, rotation_quat, scale);

		if (go != nullptr)
			global_transform_mat = go->GetComponentTransform()->GetGlobalTransform() * transform_mat;

		has_transformed = false;
	}
}

// ------------GETTERS--------------

float4x4 ComponentTransform::GetTransform() const
{
	return transform_mat;
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
	return global_transform_mat;
}

// ------------SETTERS--------------

void ComponentTransform::SetPosition(float3& position)
{
	this->position = position;
	has_transformed = true;
}

void ComponentTransform::SetQuaternionRotation(Quat& rotation)
{
	this->rotation_quat = rotation;
	this->rotation_euler = rotation_quat.ToEulerXYZ() * RADTODEG;
	has_transformed = true;
}

void ComponentTransform::SetEulerRotation(float3 rot)
{
	float3 tmp = (rot - rotation_euler) * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(tmp.x, tmp.y, tmp.z);
	rotation_quat = rotation_quat * quaternion_rotation;
	rotation_euler = rot;
}

void ComponentTransform::SetScale(float3& scale)
{
	this->scale = scale;
	has_transformed = true;
}

void ComponentTransform::SetComponent(float3& position, float3& scale, Quat& rotation)
{
	SetPosition(position);
	SetScale(scale);
	SetQuaternionRotation(rotation);
}
