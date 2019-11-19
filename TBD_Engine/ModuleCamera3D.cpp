#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleEngineUI.h"

#include "mmgr/mmgr.h"



ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	main_camera = new ComponentCamera();

	main_camera->frustum.AspectRatio();
	main_camera
	Move({ 5.0f, 5.0f, 5.0f });
	//LookAt({ -100.0f, -100.0f, -100.0f });
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	App->LogInConsole("Setting up the camera");
	bool ret = true;

	App->renderer3D->camera = main_camera;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	App->LogInConsole("Cleaning camera");
	RELEASE(main_camera);

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	if (App->gui->is_game_focused)
	{

		float3 newPos(0, 0, 0);
		float speed = 3.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 8.0f * dt;

		//WASD Camera movement
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += main_camera->frustum.front * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= main_camera->frustum.front * speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= main_camera->frustum.WorldRight() * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += main_camera->frustum.WorldRight() * speed;

			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;

			
		}
		Move(newPos);
		
		//Camera pan
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (dx != 0)
			{
				//Rotate arround the Y axis to rotate in the X coord axis
				Quat quat = Quat::RotateY(dx * dt * speed);
				main_camera->frustum.up = quat.Mul(main_camera->frustum.up).Normalized();
				main_camera->frustum.front = quat.Mul(main_camera->frustum.front).Normalized();
			}

			if (dy != 0)
			{
				//Rotate arround the X local axis to rotate in the Y coord axis
				Quat quat = Quat::RotateAxisAngle(main_camera->frustum.WorldRight(), dy * dt * speed);
				float3 up = quat.Mul(main_camera->frustum.up).Normalized();
				//Cap that you can be upside down in engine
				if (up.y > 0.0f) {
					main_camera->frustum.up = up;
					main_camera->frustum.front = quat.Mul(main_camera->frustum.front).Normalized();
				}
			}
			LookAt(newPos);
		}

		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			if (dx != 0)
			{
				Quat quat = Quat::RotateY(dx*dt*Sensitivity);
				main_camera->frustum.up = quat.Mul(main_camera->frustum.up).Normalized();
				main_camera->frustum.front = quat.Mul(main_camera->frustum.front).Normalized();
			}

			if (dy != 0)
			{
				Quat quat = Quat::RotateAxisAngle(main_camera->frustum.WorldRight(), dy* dt *Sensitivity);
				float3 up = quat.Mul(main_camera->frustum.up).Normalized();

				if (up.y > 0.0f)
				{
					main_camera->frustum.up = up;
					main_camera->frustum.front = quat.Mul(main_camera->frustum.front).Normalized();
				}
			}

		}
		//Mouse Drag movement
		if(true)
		{
			float3 newPos(0, 0, 0);

			newPos += main_camera->frustum.front * speed * App->input->GetMouseZ();
			Move(newPos);
		}
	}


	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference)
{
	main_camera->frustum.pos = Position;
	LookAt(Reference);
}

// -----------------------------------------------------------------

void ModuleCamera3D::LookAt(const float3 &Spot)
{
	float3 dir = Spot - main_camera->frustum.pos;
	dir.Normalize();
	float3x3 mat = float3x3::LookAt(main_camera->frustum.front, dir, main_camera->frustum.up, float3::unitY);
	main_camera->frustum.front = mat.MulDir(main_camera->frustum.front).Normalized();
	main_camera->frustum.up = mat.MulDir(main_camera->frustum.up).Normalized();

}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	main_camera->frustum.Translate(Movement);
}
