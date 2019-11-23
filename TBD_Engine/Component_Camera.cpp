#include "Component_Camera.h"
#include "Globals.h"
#include "Application.h"
#include "Component_Mesh.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


#include "mmgr/mmgr.h"

ComponentCamera::ComponentCamera(GameObject* gameobj) : Component(Component::ComponentType::Camera, gameobj)
{
	aspect_ratio = 1.7778f;   // 16 / 9
	frustum.type = math::PerspectiveFrustum;
	frustum.front = math::float3::unitZ;
	frustum.up = math::float3::unitY;
	frustum.pos = math::float3::zero;
	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 50.0f;
	frustum.verticalFov = 60 * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(aspect_ratio * tanf(frustum.verticalFov * 0.5f));

}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update()
{
	if(!frustum_view)
		DrawFrustum();
}

void ComponentCamera::LookAt(const float3 & position)
{
	float3 looking = position - frustum.pos;

	float3x3 tmp = float3x3::LookAt(frustum.front, looking.Normalized(), frustum.up, float3::unitY);

	frustum.front = tmp.MulDir(frustum.front).Normalized();
	frustum.up = tmp.MulDir(frustum.up).Normalized();
}

float ComponentCamera::GetFOV() const
{
	return  frustum.verticalFov * RADTODEG;
}

float ComponentCamera::GetNearPlane() const
{
	return frustum.nearPlaneDistance;
}

float ComponentCamera::GetFarPlane() const
{
	return frustum.farPlaneDistance;
}

float ComponentCamera::GetAspectRatio() const
{
	return aspect_ratio;
}

float4x4 ComponentCamera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

float4x4 ComponentCamera::GetProjectionMatrix() const
{
	return float4x4::D3DPerspProjRH(frustum.nearPlaneDistance, frustum.farPlaneDistance, frustum.NearPlaneWidth(), frustum.NearPlaneHeight());
}

float4x4 ComponentCamera::GetOpenGLView() const
{
	return float4x4(frustum.ViewMatrix()).Transposed();
}

float4x4 ComponentCamera::GetOpenGLProjection() const
{
	return frustum.ProjectionMatrix().Transposed();
}

void ComponentCamera::SetFOV(float fov)
{
	frustum.verticalFov = DEGTORAD * fov;
	frustum.horizontalFov = 2.0f * atanf(aspect_ratio * tanf(frustum.verticalFov * 0.5f));

	has_transformed = true;
}

void ComponentCamera::SetAspectRatio(float aspect)
{
	aspect_ratio = aspect;
	frustum.horizontalFov = 2.0f * atanf(aspect_ratio * tanf(frustum.verticalFov * 0.5f));
	has_transformed = true;
}

void ComponentCamera::SetNearPlane(float near_plane)
{
	if (near_plane >= frustum.farPlaneDistance)
		near_plane = frustum.farPlaneDistance - 1.0f;

	if (near_plane < 0.0f)
		near_plane = 0.1f;

	frustum.nearPlaneDistance = near_plane;

	has_transformed = true;
}

void ComponentCamera::SetFarPlane(float far_plane)
{
	if (far_plane <= frustum.nearPlaneDistance)
		far_plane = frustum.nearPlaneDistance + 1.0f;

	frustum.farPlaneDistance = far_plane;

	has_transformed = true;
}

void ComponentCamera::UpdateTransform()
{
	frustum.pos = my_GO->GetComponentTransform()->GetPosition();
	frustum.front = my_GO->GetComponentTransform()->GetGlobalTransform().WorldZ();
	frustum.up = my_GO->GetComponentTransform()->GetGlobalTransform().WorldY();

	UpdatePlanes();
}

void ComponentCamera::UpdatePlanes()
{
	frustum.GetPlanes(planes);
}

void ComponentCamera::DrawFrustum()
{
	glBegin(GL_LINES);
	glLineWidth(2.0f);
	glColor4f(Red.r, Red.g, Red.b, Red.a);

	for (uint i = 0; i < frustum.NumEdges(); i++)
	{
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
	}

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

bool ComponentCamera::ContainsAABB(const AABB & reference)
{
	return ContainsAABB(frustum, reference);
}

bool ComponentCamera::ContainsAABB(const Frustum & frustum, const AABB & reference)
{
	float3 corners[8];
	reference.GetCornerPoints(corners);

	Plane p[6];
	frustum.GetPlanes(p);

	uint totalInside = 0;

	for (int i = 0; i < 6; ++i)
	{	
		int insideCount = 8;

		for (int j = 0; j < 8; ++j)
			if (p[i].IsOnPositiveSide(corners[j]))	
				--insideCount;

		if (insideCount == 0)
			return false;	
		else
			totalInside += 1;
	}

	if (totalInside == 6)
		return true;	
	else
		return true;	
}

