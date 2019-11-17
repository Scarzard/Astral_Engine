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
	CalculateViewMatrix();

	//frustum_vertices = new float3[8];
	frustum.pos = { 0,0,0 };
	frustum.up = { 0,1,0 };
	frustum.front = { 0,0,1 };

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ComponentCamera::~ComponentCamera()
{

}


void ComponentCamera::CameraControl()
{
	if (App->gui->is_game_focused)
	{

		vec3 newPos(0, 0, 0);
		float speed = 3.0f;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 8.0f;



		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;
		}
		else
		{
			if (App->input->GetMouseZ() > 0) newPos -= Z * speed * 15;
			if (App->input->GetMouseZ() < 0) newPos += Z * speed * 15;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
		{
			if (-App->input->GetMouseXMotion() > 0) newPos += X * speed * 2;
			if (-App->input->GetMouseXMotion() < 0) newPos -= X * speed * 2;

			if (-App->input->GetMouseYMotion() > 0) newPos.y -= speed * 2;
			if (-App->input->GetMouseYMotion() < 0) newPos.y += speed * 2;
		}

		Position += newPos;
		Reference += newPos;

		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
	}
}
// -----------------------------------------------------------------
void ComponentCamera::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ComponentCamera::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ComponentCamera::GetViewMatrix()
{
	return &ViewMatrix;
}


// -----------------------------------------------------------------
void ComponentCamera::CalculateViewMatrix()
{
	
	ViewMatrix = mat4x4({ X.x, Y.x, Z.x, 0.0f },	
						{ X.y, Y.y, Z.y, 0.0f },
						{ X.z, Y.z, Z.z, 0.0f }, 
						{ -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f });

	frustum.SetWorldMatrix(float3x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f));
}
