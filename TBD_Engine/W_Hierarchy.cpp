#include "Application.h"
#include "W_Hierarchy.h"

W_Hierarchy::W_Hierarchy() : Window()
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
	if (App->gui->hierarchy)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Hierarchy");
		//Draw Hierarchy stuff
		for (std::vector<GameObject*>::iterator iterator = App->scene_intro->GO_list.begin(); iterator != App->scene_intro->GO_list.end(); iterator++)
		{
			
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	
			if ((*iterator)->active = false)
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);


			ImGui::TreeNodeEx((void*)(intptr_t)(*iterator)->id, node_flags, (*iterator)->name.c_str());
			
		}


		ImGui::End();
		ImGui::PopStyleVar();

	}
	
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



