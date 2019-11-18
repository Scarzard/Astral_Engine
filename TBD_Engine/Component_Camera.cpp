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
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = 60 * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(aspect_ratio * tanf(frustum.verticalFov * 0.5f));

}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update()
{
	if(frustrum_view)
		DrawFrustum();
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

// -----------------------------------------------------------------
float* ComponentCamera::GetViewMatrix()
{
	view_mat = frustum.ViewMatrix();
	view_mat.Transpose();

	return (float*)view_mat.v;
}

float * ComponentCamera::GetProjectionMatrix()
{
	projection_mat = frustum.ProjectionMatrix();
	projection_mat.Transpose();

	return (float*)projection_mat.v;
}


void ComponentCamera::SetFOV(float fov)
{
	frustum.verticalFov = DEGTORAD * fov;
	frustum.horizontalFov = 2.0f * atanf(aspect_ratio * tanf(frustum.verticalFov * 0.5f));
}

void ComponentCamera::SetAspectRatio(float aspect)
{
	aspect_ratio = aspect;
	frustum.horizontalFov = 2.0f * atanf(aspect_ratio * tanf(frustum.verticalFov * 0.5f));
}

void ComponentCamera::SetNearPlane(float near_plane)
{
	if (near_plane >= frustum.farPlaneDistance)
		near_plane = frustum.farPlaneDistance - 1.0f;

	if (near_plane < 0.0f)
		near_plane = 0.1f;

	frustum.nearPlaneDistance = near_plane;
}

void ComponentCamera::SetFarPlane(float far_plane)
{
	if (far_plane <= frustum.nearPlaneDistance)
		far_plane = frustum.nearPlaneDistance + 1.0f;

	frustum.farPlaneDistance = far_plane;
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
