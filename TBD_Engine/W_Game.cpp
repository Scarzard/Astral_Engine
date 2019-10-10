#include "Application.h"
#include "W_Game.h"
#include "ModuleRenderer3D.h"
#include "glew/include/GL/glew.h"


W_Game::W_Game(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

W_Game::~W_Game()
{
}

bool W_Game::Start()
{
	current_size = { 1024, 768 };
	fbo = new FrameBufferObject();
	fbo->Start(current_size);

	return true;
}

bool W_Game::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Game");
	ImGui::SetWindowSize(current_size);

	//1. Create aFrame Buffer Object
	//2. Texture obtained from step 1
	//3. We need to put the image in ImGui::Image
	//ImGui::Image(texture, size);

	App->renderer3D->OnResize(current_size.x, current_size.y);
	ImGui::Image((ImTextureID)fbo->texture, ImVec2(current_size.x, current_size.y), ImVec2(0, 1), ImVec2(1, 0));


	ImGui::End();
	ImGui::PopStyleVar();

	return true;
}

update_status W_Game::PreUpdate(float dt) 
{
	fbo->PreUpdate();
	return UPDATE_CONTINUE;
}

update_status W_Game::PostUpdate(float dt)
{
	fbo->PostUpdate();
	return UPDATE_CONTINUE;
}

bool W_Game::Cleanup()
{
	fbo->CleanUp();
	return true;
}