#include "Application.h"
#include "SceneGame.h"
#include "ModuleRenderer3D.h"
#include "glew/include/GL/glew.h"


SceneGame::SceneGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

SceneGame::~SceneGame()
{
}


bool SceneGame::Start()
{
	current_size = { 1024, 768 };
	fbo = new FrameBufferObject();
	fbo->Start(current_size);

	return true;
}

update_status SceneGame::PreUpdate(float dt) 
{
	fbo->PreUpdate();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Game");
	ImGui::SetWindowSize(current_size);


	//1. Create aFrame Buffer Object
	//2. Texture obtained from step 1
	//3. We need to put the image in ImGui::Image
	//ImGui::Image(texture, size);

	ImGui::End();
	ImGui::PopStyleVar();

	return UPDATE_CONTINUE;
}

update_status SceneGame::PostUpdate(float dt)
{
	fbo->PostUpdate();
	return UPDATE_CONTINUE;
}

bool SceneGame::Cleanup()
{
	fbo->CleanUp();

	return true;
}
