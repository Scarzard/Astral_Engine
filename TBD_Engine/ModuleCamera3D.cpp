#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleEngineUI.h"
#include "GameObject.h"
#include "ModuleEngineUI.h"
#include "ModuleSceneIntro.h"
#include "SDL/include/SDL_opengl.h"
#include "ModuleInput.h"

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
	main_camera->frustum.farPlaneDistance = 300;

	App->LogInConsole("Setting up the camera");
	bool ret = true;

	obj_camera = App->scene_intro->CreateGameObject();
	obj_camera->name = "Main Camera";
	obj_camera->is_static = false;
	obj_camera->CreateComponent(Component::ComponentType::Camera);

	//------ Starting position of camera
	obj_camera->GetComponentTransform()->SetEulerRotation(float3(-180, 0, -180));
	obj_camera->GetComponentTransform()->SetPosition(float3(0, 14, 30));
	
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
		if (activate_mouse_picking && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && (App->input->GetMouseX() > App->gui->game_window->position.x && App->input->GetMouseX() < App->gui->game_window->position.x + App->gui->game_window->current_size.x) &&
			(App->input->GetMouseY() > App->gui->game_window->position.y && App->input->GetMouseY() < App->gui->game_window->position.y + App->gui->game_window->current_size.y))
			MouseClick();

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
	DrawRay();

	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	active_camera->frustum.pos += Movement;
	looking_at += Movement;
}

void ModuleCamera3D::LookAt(const float3 & spot, float dist)
{
	looking_at = spot;
	active_camera->LookAt(looking_at);

	if (dist > 0.f)
		active_camera->frustum.pos = looking_at - active_camera->frustum.front * dist;
}

void ModuleCamera3D::CenterToObject(GameObject * obj)
{
	/*if (obj != nullptr) {
		float dist = Length({ 10.0f, 10.0f, 10.0f });

		ComponentTransform* transform = obj->GetComponentTransform();
		if (transform != nullptr)
			looking_at = transform->position;

		ComponentMesh* mesh = obj->GetComponentMesh();
		if (mesh != nullptr)
			dist = Length(float3(mesh., mesh->mesh->size.y, mesh->mesh->size.z)) * Length(float3(transform->scale.x, transform->scale.y, transform->scale.z));

		LookAt(looking_at, dist);
	}*/
}

float3 ModuleCamera3D::GetPosition() const
{
	return active_camera->frustum.pos;
}

float * ModuleCamera3D::GetOpenGLView() const
{
	return *active_camera->GetOpenGLView().v;
}

float * ModuleCamera3D::GetOpenGLProjection() const
{
	return *active_camera->GetOpenGLProjection().v;
}

bool * ModuleCamera3D::GetProjectionUpdateFlag() const
{
	return &active_camera->has_transformed;
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
	return obj_camera->GetComponentCamera()->ContainsAABB(refBox);
}

void ModuleCamera3D::MoveCamera(float & movSpeed)
{
	float3 mov(float3::zero);

	float3 right(active_camera->frustum.WorldRight());
	float3 front(active_camera->frustum.front);

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
		active_camera->frustum.Translate(mov * movSpeed);
	}
}

void ModuleCamera3D::DragCamera(float delta_x, float delta_y)
{
	float3 mov(float3::zero);

	mov += active_camera->frustum.WorldRight() * delta_x;
	mov -= active_camera->frustum.up * delta_y;

	active_camera->frustum.Translate(mov * 100.f);
	looking_at += mov * 100.f;
}

void ModuleCamera3D::Zoom(float delta_z)
{
	active_camera->frustum.pos += active_camera->frustum.front * delta_z;
}

bool ModuleCamera3D::FirstPersonCamera(float & movSpeed)
{
	bool ret = false;
	

	return ret;
}

void ModuleCamera3D::MouseClick()
{
	vec2 mousePos(float(App->input->GetMouseX() - App->gui->game_window->position.x - 5), float(App->input->GetMouseY() - App->gui->game_window->position.y - 30));
	mousePos.x = -1.0 + 2.0f*(mousePos.x / App->gui->game_window->current_size.x);
	mousePos.y = 1.0 - 2.0f*(mousePos.y / (App->gui->game_window->current_size.x / App->camera->active_camera->GetAspectRatio()));

	App->camera->OnClick(mousePos);

	App->gui->ins_window->selected_GO = App->scene_intro->CollectHits();
}

void ModuleCamera3D::OnClick(const vec2 & normMousePos)
{
	hit = active_camera->frustum.UnProjectLineSegment(normMousePos.x, normMousePos.y);
}

void ModuleCamera3D::Orbit(float motion_x, float motion_y)
{
	float3 focus = active_camera->frustum.pos - looking_at;

	Quat qy(active_camera->frustum.up, motion_x);
	Quat qx(active_camera->frustum.WorldRight(), motion_y);

	focus = qx.Transform(focus);
	focus = qy.Transform(focus);

	active_camera->frustum.pos = focus + looking_at;

	LookAt(looking_at);
}

void ModuleCamera3D::Rotate(float motion_x, float motion_y)
{
	// X motion make the camera rotate in Y global axis
	if (motion_x != 0.f)
	{
		Quat q = Quat::RotateY(motion_x);
		active_camera->frustum.front = q.Mul(active_camera->frustum.front).Normalized();
		active_camera->frustum.up = q.Mul(active_camera->frustum.up).Normalized();
	}

	// Y motion makes the camera rotate in X local axis
	if (motion_y != 0.f)
	{
		Quat q = Quat::RotateAxisAngle(active_camera->frustum.WorldRight(), motion_y);
		float3 newPos = q.Mul(active_camera->frustum.up).Normalized();

		if (newPos.y > 0.0f)
		{
			active_camera->frustum.up = newPos;
			active_camera->frustum.front = q.Mul(active_camera->frustum.front).Normalized();
		}
	}

	float3 dist = looking_at - active_camera->frustum.pos;
	looking_at = active_camera->frustum.pos + active_camera->frustum.front * dist.Length();
}

void ModuleCamera3D::DrawRay()
{
	glBegin(GL_LINES);
	glLineWidth(2.0f);
	glColor4f(Red.r, Red.g, Red.b, Red.a);

	
	glVertex3f(hit.a.x, hit.a.y, hit.a.z);
	glVertex3f(hit.b.x, hit.b.y, hit.b.z);

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

