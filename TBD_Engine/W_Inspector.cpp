#include "Application.h"
#include "W_Inspector.h"
#include "Component_Transform.h"

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
	if (App->gui->inspector)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Inspector");
		//Draw Inspector stuff

		float x, y, z;

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position:");
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("X", &x, 0.005f);
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("Y", &y, 0.005f);
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("Z", &z, 0.005f);

			ImGui::Text("Rotation:");
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("X", &x, 0.005f);
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("Y", &y, 0.005f);
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("Z", &z, 0.005f);

			ImGui::Text("Scale:   ");
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("X", &x, 0.005f);
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("Y", &y, 0.005f);
			ImGui::SameLine(); ImGui::PushItemWidth(70); ImGui::DragFloat("Z", &z, 0.005f);
		}

		if (ImGui::CollapsingHeader("Mesh"))
		{

		}

		if (ImGui::CollapsingHeader("Texture"))
		{

		}

		


		ImGui::End();
		ImGui::PopStyleVar();

	}
	
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


