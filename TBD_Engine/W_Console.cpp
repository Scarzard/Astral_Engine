#include "Application.h"
#include "W_Console.h"

W_Console::W_Console(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

W_Console::~W_Console()
{
}

bool W_Console::Start()
{
	return true;
}

bool W_Console::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Console");
	//Draw Inspector stuff

	ImGui::End();
	ImGui::PopStyleVar();

	return true;
}

update_status W_Console::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status W_Console::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool W_Console::CleanUp()
{
	return true;
}


