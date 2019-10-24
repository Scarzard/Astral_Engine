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

		if (ImGui::CollapsingHeader("Transform"))
		{

			float3 pos = float3::zero;
			float3 rot = float3::zero;
			float3 scale = float3::one;

			ImGui::Text("");

			ImGui::Text("Position:");
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("X1", &pos.x, 0.05f);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("Y1", &pos.y, 0.05f);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("Z1", &pos.z, 0.05f);
													
			ImGui::Text("Rotation:");				
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("X2", &rot.x, 0.05f);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("Y2", &rot.y, 0.05f);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("Z2", &rot.z, 0.05f);
													
			ImGui::Text("Scale:   ");				
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("X3", &scale.x, 0.05f);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("Y3", &scale.y, 0.05f);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("Z3", &scale.z, 0.05f);
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


