#include "Application.h"
#include "W_Inspector.h"
#include "Component_Transform.h"
#include "Component_Camera.h"
#include "SpacePartition.h"
#include "ModuleEngineUI.h"
#include "ModuleSceneIntro.h"
#include "ResourceTexture.h"


W_Inspector::W_Inspector() : Window()
{
	App->LogInConsole("Created Inspector Window");
}

W_Inspector::~W_Inspector()
{
}


bool W_Inspector::Start()
{
	//selected_GO = selected_GO.
	return true;
}

bool W_Inspector::Draw()
{
	if (App->gui->inspector)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7, 3));

		ImGui::Begin("Inspector");
		//Draw Inspector stuff
		if (selected_GO != nullptr && selected_GO->id != 0)
		{
			ImGui::InputText("Name", (char*)selected_GO->name.c_str(), 20, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::Text("");
			active_bool = selected_GO->active;
			ImGui::Checkbox("Active", &active_bool);

			if (ImGui::Checkbox("Static", &selected_GO->is_static))
			{
				if (selected_GO->GetComponentMesh() != nullptr)
				{
					ComponentMesh* mesh = selected_GO->GetComponentMesh();
					if (selected_GO->is_static)
					{
						//TODO: add mesh to static_meshes && update Octree ?
						App->scene_intro->static_meshes.push_back(mesh);
						App->scene_intro->QuadTree->update_tree = true;
					}
					else
					{
						//TODO: remove mesh from static_meshes && update Octree 
						
						for (std::vector<ComponentMesh*>::iterator it = App->scene_intro->static_meshes.begin(); it != App->scene_intro->static_meshes.end(); it++)
						{
							if ((*it)->my_GO->id == selected_GO->id)
							{
								App->scene_intro->static_meshes.erase(it);
								break;
							}
						}
						App->scene_intro->QuadTree->update_tree = true;
					}
				}
				else
				{
					App->scene_intro->QuadTree->update_tree = true;
				}
				
			}

			if (active_bool)
			{
				selected_GO->Enable();
			}
			else
			{
				selected_GO->Disable();
			}

			if (selected_GO->active)
			{
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_Leaf) && selected_GO->GetComponentTransform() != nullptr)
				{
					ComponentTransform* transform = selected_GO->GetComponentTransform();

					if (ImGui::Button("Reset transform")) if(!selected_GO->is_static)transform->ResetPosition();
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(" Position: (0, 0, 0) \n Rotation: (0, 0, 0) \n Scale:    (1, 1, 1)");

					float3 position = transform->GetPosition();
					ImGui::Text("Position:");

					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##X1", &position.x, 0.05f, -INFINITY, INFINITY);
					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##Y1", &position.y, 0.05f, -INFINITY, INFINITY);
					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##Z1", &position.z, 0.05f, -INFINITY, INFINITY);
					if (!transform->GetPosition().Equals(position) && !selected_GO->is_static)
						transform->SetPosition(position);

					ImGui::Text("");

					float3 rotation = transform->GetEulerRotation();
					ImGui::Text("Rotation:");
																				   
					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##X2", &rotation.x, 0.05f, -INFINITY, INFINITY);
					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##Y2", &rotation.y, 0.05f, -INFINITY, INFINITY);
					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##Z2", &rotation.z, 0.05f, -INFINITY, INFINITY);
					if (!transform->GetEulerRotation().Equals(rotation) && !selected_GO->is_static)
						transform->SetEulerRotation(rotation);
					ImGui::Text("");

					float3 sc = transform->GetScale();
					ImGui::Text("Scale:   ");
					
					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##X3", &sc.x, 0.05f, -INFINITY, INFINITY);
					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##Y3", &sc.y, 0.05f, -INFINITY, INFINITY);
					ImGui::SameLine(); ImGui::PushItemWidth(50); ImGui::DragFloat("##Z3", &sc.z, 0.05f, -INFINITY, INFINITY);
	
					if (!transform->GetScale().Equals(sc) && !selected_GO->is_static)
						transform->SetScale(sc);
					
				}

				if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_Leaf) && selected_GO->GetComponentMesh() != nullptr)
				{
					ImGui::Text("Vertex:");
					ImGui::SameLine(); ImGui::Text("%d", selected_GO->GetComponentMesh()->res_mesh->num_vertex);
					ImGui::Text("Faces:");
					ImGui::SameLine(); ImGui::Text("%d", (selected_GO->GetComponentMesh()->res_mesh->num_vertex / 3));
					ImGui::Checkbox("Show normals", &selected_GO->GetComponentMesh()->draw_normals);
				}

				if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_Leaf) && selected_GO->GetComponentTexture() != nullptr)
				{
					ComponentTexture* comp_tex = selected_GO->GetComponentTexture();

					if (ImGui::Checkbox("Checkers Texture", &selected_GO->GetComponentTexture()->Checers_texture)){}

					if (selected_GO->GetComponentTexture()->Checers_texture == false)
					{
						if (comp_tex->res_texture != nullptr)
						{
							ImGui::Text("");
							ImGui::Text("Texture path: %s", comp_tex->res_texture->file.c_str());
							ImGui::Text("");
							ImGui::Text("Texture Width: %u", comp_tex->res_texture->width);
							ImGui::Text("");
							ImGui::Text("Texture Height: %u", comp_tex->res_texture->height);
							ImGui::Text("");
							ImGui::Text("Texture preview:");
							ImGui::Image((ImTextureID*)comp_tex->res_texture->texture, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
						}
						else
						{
							ImGui::Text("");
							ImGui::Text("No texture applied");
						}

					}
					else if (selected_GO->GetComponentTexture()->Checers_texture == true)
					{
						ImGui::Text("");
						ImGui::Text("Texture path: %s", App->tex_loader->CheckersTexture.path.c_str());
						ImGui::Text("");
						ImGui::Text("Texture Width: %u", App->tex_loader->CheckersTexture.width);
						ImGui::Text("");
						ImGui::Text("Texture Height: %u", App->tex_loader->CheckersTexture.height);
						ImGui::Text("");
						ImGui::Text("Texture preview:");
						ImGui::Image((ImTextureID*)App->tex_loader->CheckersTexture.id, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
					}
				}
				////Hardcoded camera inspector
				

				//Non hardcode inspector for camera
				if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_Leaf) && selected_GO->GetComponentCamera() != nullptr)
				{
					if (ImGui::Checkbox("Toggle frustum draw", &selected_GO->GetComponentCamera()->frustum_view));
					if (ImGui::Checkbox("Toggle frustum culling", &selected_GO->GetComponentCamera()->culling));

					ComponentCamera* camera = selected_GO->GetComponentCamera();
					ImGui::Text("Horizontal FOV:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.0f, 1.0f), "%f", camera->frustum.horizontalFov);


					ImGui::Text("Vertical FOV:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.0f, 1.0f), "%f", camera->frustum.verticalFov);

					float fov = camera->frustum.verticalFov * RADTODEG;
					ImGui::Text("Set FOV:       ");
					ImGui::SameLine(); ImGui::PushItemWidth(100);
					if (ImGui::DragFloat("1", &fov, 1.0f))
						camera->SetFOV(fov);

					float near_plane = camera->frustum.nearPlaneDistance;
					ImGui::Text("Set Near Plane:");
					ImGui::SameLine(); ImGui::PushItemWidth(100);
					if (ImGui::DragFloat("2", &near_plane, 1.0F, -INFINITY, INFINITY))
					{
						camera->SetNearPlane(near_plane);
					}

					float far_plane = camera->frustum.farPlaneDistance;
					ImGui::Text("Set Far Plane: ");
					ImGui::SameLine(); ImGui::PushItemWidth(100);
					if (ImGui::DragFloat("3", &far_plane, 1.0F, -INFINITY, INFINITY))
					{
						camera->SetFarPlane(far_plane);
					}
				}
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

	}
	
	return true;
}


