#include "Component_Transform.h"
#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "mmgr/mmgr.h"

ComponentTransform::ComponentTransform(GameObject* GO, float3 position, Quat rotation, float3 scale) : Component(Component::ComponentType::Transform, GO)
{
	transform = float4x4::FromTRS(position, rotation, scale);
}

ComponentTransform::~ComponentTransform()
{

}

void ComponentTransform::CleanUp()
{

}

float4x4 ComponentTransform::GetTransform() const
{
	return transform;
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

void ComponentTransform::SetPosition(float3& position)
{
	this->position = position;
}

void ComponentTransform::SetQuaternionRotation(Quat& rotation)
{
	this->rotation_quat = rotation;
	this->rotation_euler = rotation_quat.ToEulerXYZ() * RADTODEG;
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
}

void ComponentTransform::SetComponent(float3& position, float3& scale, Quat& rotation)
{
	SetPosition(position);
	SetScale(scale);
	SetQuaternionRotation(rotation);
}
