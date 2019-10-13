#include "Application.h"
#include "W_Inspector.h"

W_Inspector::W_Inspector() : Window()
{
}

W_Inspector::~W_Inspector()
{
}


bool W_Inspector::Start()
{
	return true;
}

bool W_Inspector::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Inspector");
	//Draw Inspector stuff

	ImGui::End();
	ImGui::PopStyleVar();

	return true;
}

update_status W_Inspector::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status W_Inspector::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool W_Inspector::CleanUp()
{
	return true;
}


