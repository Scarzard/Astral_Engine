#include "Application.h"
#include "W_Console.h"

W_Console::W_Console() : Window()
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
	
	for (int i = 0; i < App->Logs_Console.size(); ++i)
		ImGui::Text(App->Logs_Console[i].data());
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


