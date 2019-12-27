#include "Component_Bone.h"
#include "GameObject.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Color.h"

ComponentBone::ComponentBone(GameObject * gameObject) : Component(Component::ComponentType::Bone, gameObject)
{
}

ComponentBone::~ComponentBone()
{
}

void ComponentBone::DebugDrawBones()
{
	if (my_GO->parent != nullptr && my_GO->parent->GetComponentBone() != nullptr)
	{
		float4x4 child_matrix = GetBoneTransform();
		float4x4 parent_matrix = my_GO->parent->GetComponentBone()->GetBoneTransform();
		float3 child_pos = float3(child_matrix.At(0, 3), child_matrix.At(1, 3), child_matrix.At(2, 3));
		float3 parent_pos = float3(parent_matrix.At(0, 3), parent_matrix.At(1, 3), parent_matrix.At(2, 3));

		glBegin(GL_LINES);
		glLineWidth(2.0f);
		glColor4f(Yellow.r, Yellow.g, Yellow.b, Yellow.a);

		glVertex3f(child_pos.x, child_pos.y, child_pos.z);
		glVertex3f(parent_pos.x, parent_pos.y, parent_pos.z);


		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
}

float4x4 ComponentBone::GetBoneTransform()
{
	return my_GO->GetComponentTransform()->GetGlobalTransform();
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
