#include "Application.h"
#include "W_Hierarchy.h"
#include "ModuleEngineUI.h"
#include "ModuleSceneIntro.h"

W_Hierarchy::W_Hierarchy() : Window()
{
	App->LogInConsole("Created Hierarchy Window");
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

		DrawRecursively(App->scene_intro->root);

		ImGui::End();
		ImGui::PopStyleVar();

	}
	
	return true;
}

void W_Hierarchy::DrawRecursively(GameObject* GO)
{
	
	static ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
	ImGuiTreeNodeFlags node_falg = flag;

	if(GO == App->gui->ins_window->selected_GO)
		node_falg |= ImGuiTreeNodeFlags_Selected;

	bool node_open = false;

	if (GO->active == true)
		node_open = ImGui::TreeNodeEx((void*)(intptr_t)GO->id, node_falg, GO->name.c_str());
	else
		node_open = ImGui::TreeNodeEx((void*)(intptr_t)GO->id, node_falg, GO->unactive_name.c_str());


	// when node is clicked
	if (ImGui::IsItemClicked())
	{
		App->gui->ins_window->selected_GO = GO;
	}

	//if node is opened draw his childs
	if (node_open) 
	{
		// only if it has childs 
		if (GO->children.size() > 0 && GO->active)
		{
			for (std::vector<GameObject*>::iterator iterator = GO->children.begin(); iterator != GO->children.end(); iterator++)
			{
				DrawRecursively(*iterator);
			}
		}

		ImGui::TreePop();
	}
}
