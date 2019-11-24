#include "Application.h"
#include "W_Resources.h"
#include "ModuleEngineUI.h"
#include "ModuleResources.h"
W_Resources::W_Resources() : Window()
{
}

W_Resources::~W_Resources()
{
	App->LogInConsole("Created Resources Window");
}

bool W_Resources::Start()
{
	return true;
}

bool W_Resources::Draw()
{
	
	if (App->gui->w_resources)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

		ImGui::Begin("Resources");
		ImGui::Separator(); ImGui::Text("Resources Explorer: Hover a resource to see information about it"); ImGui::Separator();

		App->resources->DrawExplorer();

		ImGui::End();
		ImGui::PopStyleVar();
	}

	return true;
}