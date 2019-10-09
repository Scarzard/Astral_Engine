#include "Application.h"
#include "W_Hierarchy.h"

W_Hierarchy::W_Hierarchy(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

W_Hierarchy::~W_Hierarchy()
{
}

bool W_Hierarchy::Start()
{
	return true;
}

bool W_Hierarchy::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Hierarchy");
	//Draw Hierarchy stuff

	ImGui::End();
	ImGui::PopStyleVar();

	return true;
}

update_status W_Hierarchy::PreUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

update_status W_Hierarchy::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

bool W_Hierarchy::CleanUp()
{
	return true;
}



