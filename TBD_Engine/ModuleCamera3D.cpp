#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleEngineUI.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"



ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	active_camera = main_camera = new ComponentCamera();
}

ModuleCamera3D::~ModuleCamera3D()
{
	delete main_camera;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	App->LogInConsole("Setting up the camera");
	bool ret = true;

	obj_camera = App->scene_intro->CreateGameObject();
	obj_camera->name = "Main Camera";
	obj_camera->CreateComponent(Component::ComponentType::Camera);
	
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
		float currMovSpeed = 4.0f * dt;
		float slowingFactor = 2.0;

		float3 speed = {(float)-App->input->GetMouseXMotion() * slowingFactor *  dt, (float)-App->input->GetMouseYMotion() * slowingFactor * dt, (float)App->input->GetMouseZ() * slowingFactor * dt };

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
				Orbit(speed.x, speed.y);											//Orbit center
			else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
				Zoom(-speed.y);		                                                // Zoom Camera
		}

		else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			Rotate(speed.x / slowingFactor, speed.y / slowingFactor);				//Rotate normally
			MoveCamera(currMovSpeed);												//WASD movement
		}

		// Center camera to object
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) 
		{	
			if (App->gui->ins_window->selected_GO != nullptr) 
			{
				CenterToObject(App->gui->ins_window->selected_GO);
			}
		}
		
	}

	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	main_camera->frustum.pos += Movement;
	looking_at += Movement;
}

void ModuleCamera3D::LookAt(const float3 & spot, float dist)
{
	looking_at = spot;
	main_camera->LookAt(looking_at);

	if (dist > 0.f)
		main_camera->frustum.pos = looking_at - main_camera->frustum.front * dist;
}

void ModuleCamera3D::CenterToObject(GameObject * obj)
{
	if (obj != nullptr) {
		float dist = Length({ 10.0f, 10.0f, 10.0f });

		ComponentTransform* transform = obj->GetComponentTransform();
		if (transform != nullptr)
			looking_at = transform->position;

		ComponentMesh* mesh = obj->GetComponentMesh();
		if (mesh != nullptr)
			dist = Length(float3(transform->scale.x, transform->scale.y, transform->scale.z));

		LookAt(looking_at, dist);
	}
}

float3 ModuleCamera3D::GetPosition() const
{
	return main_camera->frustum.pos;
}

float * ModuleCamera3D::GetOpenGLView() const
{
	return *main_camera->GetOpenGLView().v;
}

float * ModuleCamera3D::GetOpenGLProjection() const
{
	return *main_camera->GetOpenGLProjection().v;
}

bool * ModuleCamera3D::GetProjectionUpdateFlag() const
{
	return &main_camera->has_transformed;
}

ComponentCamera * ModuleCamera3D::GetActiveCamera() const
{
	return active_camera;
}

const Frustum & ModuleCamera3D::GetActiveFrustum() const
{
	return active_camera->frustum;
}

bool ModuleCamera3D::Intersects(const AABB & refBox) const
{
	return false;
}

void ModuleCamera3D::MoveCamera(float & movSpeed)
{
	float3 mov(float3::zero);

	float3 right(main_camera->frustum.WorldRight());
	float3 front(main_camera->frustum.front);

	// Boost speed if not already
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) movSpeed *= 8.0;

	// Forward/Backwards
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) mov += front;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) mov -= front;

	// Left/Right
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) mov -= right;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) mov += right;

	// Down/Up
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) mov -= float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) mov += float3::unitY;

	if (!mov.Equals(float3::zero)) {
		main_camera->frustum.Translate(mov * movSpeed);
	}
}

void ModuleCamera3D::DragCamera(float delta_x, float delta_y)
{
	float3 mov(float3::zero);

	mov += main_camera->frustum.WorldRight() * delta_x;
	mov -= main_camera->frustum.up * delta_y;

	main_camera->frustum.Translate(mov * 100.f);
	looking_at += mov * 100.f;
}

void ModuleCamera3D::Zoom(float delta_z)
{
	main_camera->frustum.pos += main_camera->frustum.front * delta_z;
}

bool ModuleCamera3D::FirstPersonCamera(float & movSpeed)
{
	bool ret = false;
	

	return ret;
}

void ModuleCamera3D::Orbit(float motion_x, float motion_y)
{
	float3 focus = main_camera->frustum.pos - looking_at;

	Quat qy(main_camera->frustum.up, motion_x);
	Quat qx(main_camera->frustum.WorldRight(), motion_y);

	focus = qx.Transform(focus);
	focus = qy.Transform(focus);

	main_camera->frustum.pos = focus + looking_at;

	LookAt(looking_at);
}

void ModuleCamera3D::Rotate(float motion_x, float motion_y)
{
	// X motion make the camera rotate in Y global axis
	if (motion_x != 0.f)
	{
		Quat q = Quat::RotateY(motion_x);
		main_camera->frustum.front = q.Mul(main_camera->frustum.front).Normalized();
		main_camera->frustum.up = q.Mul(main_camera->frustum.up).Normalized();
	}

	// Y motion makes the camera rotate in X local axis
	if (motion_y != 0.f)
	{
		Quat q = Quat::RotateAxisAngle(main_camera->frustum.WorldRight(), motion_y);
		float3 newPos = q.Mul(main_camera->frustum.up).Normalized();

		if (newPos.y > 0.0f)
		{
			main_camera->frustum.up = newPos;
			main_camera->frustum.front = q.Mul(main_camera->frustum.front).Normalized();
		}
	}

	float3 dist = looking_at - main_camera->frustum.pos;
	looking_at = main_camera->frustum.pos + main_camera->frustum.front * dist.Length();
}
