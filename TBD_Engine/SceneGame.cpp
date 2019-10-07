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

	return true;
}

update_status SceneGame::PreUpdate(float dt) 
{
	ImGui::Begin("Game");
	ImGui::SetWindowSize(current_size);
	glViewport(0, 0, current_size.x, current_size.y);

	//1. Create aFrame Buffer Object
	//2. Texture obtained from step 1
	//3. We need to put the image in ImGui::Image
	//ImGui::Image(texture, size);

	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status SceneGame::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool SceneGame::Cleanup()
{
	
	return true;
}
