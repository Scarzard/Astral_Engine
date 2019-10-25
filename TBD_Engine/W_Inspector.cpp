#include "Application.h"
#include "W_Inspector.h"
#include "Component_Transform.h"

W_Inspector::W_Inspector() : Window()
{
	App->LogInConsole("Created Inspector Window");
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
		if(selected_GO != nullptr)
			ImGui::InputText("Name", (char*)selected_GO->name.c_str(), 20, ImGuiInputTextFlags_EnterReturnsTrue);

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_Leaf) && selected_GO != nullptr)
		{
			ImGui::Text("");

			ImGui::Text("Position:");
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("X1 %.3f   ", selected_GO->GetComponentTransform()->GetPosition().x);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("Y1 %.3f   ", selected_GO->GetComponentTransform()->GetPosition().y);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("Z1 %.3f   ", selected_GO->GetComponentTransform()->GetPosition().z);
			ImGui::Text("");
							
			ImGui::Text("Rotation:");				
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("X2 %.3f   ", selected_GO->GetComponentTransform()->GetQuaternionRotation().x);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("Y2 %.3f   ", selected_GO->GetComponentTransform()->GetQuaternionRotation().y);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("Z2 %.3f   ", selected_GO->GetComponentTransform()->GetQuaternionRotation().z);
			ImGui::Text("");
													
			ImGui::Text("Scale:   ");				
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("X3 %.3f   ", selected_GO->GetComponentTransform()->GetScale().x);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("Y3 %.3f   ", selected_GO->GetComponentTransform()->GetScale().y);
			ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::Text("Z3 %.3f   ", selected_GO->GetComponentTransform()->GetScale().z);
		}

		if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_Leaf) && selected_GO != nullptr)
		{
			
		}

		if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_Leaf) && selected_GO != nullptr)
		{
			ImGui::Checkbox("Checkers Texture", &selected_GO->GetComponentTexture()->Checers_texture);
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


