#include "Application.h"
#include "W_Game.h"
#include "ModuleRenderer3D.h"
#include "ModuleEngineUI.h"
#include "ModuleInput.h"
#include "glew/include/GL/glew.h"
#include "FrameBufferObject.h"
#include "TimeManager.h"

#include "mmgr/mmgr.h"

W_Game::W_Game() : Window()
{
	App->LogInConsole("Created Game Window");
}

W_Game::~W_Game()
{
}

bool W_Game::Start()
{
	current_size = { 1024, 768 };
	//MEMORY LEAK HERE
	fbo = new FrameBufferObject();
	fbo->Start(current_size);

	return true;
}

bool W_Game::Draw()
{
	if (App->gui->game)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		
		ImGui::Begin("Game");

		static char* button_name1 = "Play";
		static char* button_name2 = "Pause";
		
		static uint time_scale_width = 100;
		ENGINE_STATE engine_state = App->GetState();

		// Play/Pause
		//ImGui::SetCursorPos({ (float)(width / 2 - 35), (float)(height / 3) });

		//Play/Stop
		if (ImGui::Button(button_name1)) 
		{
			if (engine_state == ENGINE_STATE::IN_EDITOR)
			{
				if (App->Play())
				{
					button_name1 = "Stop";
					in_editor = false;
				}
			}
			else 
			{
				App->Stop();
				button_name1 = "Play";
				in_editor = true;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(button_name2)) 
		{
			if (engine_state == ENGINE_STATE::PLAY)
				button_name2 = "Resume";
			else if (engine_state == ENGINE_STATE::PAUSE)
				button_name2 = "Pause";

			App->Pause();
		}
		if (!in_editor)
		{
			float showtime = App->time->GetGameTime();
			ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.0f, 1.0f), "%f", showtime);
		}
		else
		{
			App->time->ResetGameTimer();
		}

		//
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
			App->gui->is_game_focused = ImGui::IsWindowHovered();

		//1. Create a Frame Buffer Object
		//2. Texture obtained from step 1
		//3. We need to put the image in ImGui::Image
		//ImGui::Image(texture, size);

		new_size = ImGui::GetContentRegionAvail();

		ImGui::Image((ImTextureID)fbo->texture, ImVec2(current_size.x, current_size.y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();
	}

	position = ImGui::GetWindowPos();
	
	return true;
}

update_status W_Game::PreUpdate(float dt) 
{
	if (current_size.x != new_size.x || current_size.y != new_size.y)
	{
		current_size = new_size;
		fbo->Start(current_size);
		App->renderer3D->OnResize(current_size.x, current_size.y);
	}


	fbo->PreUpdate();
	return UPDATE_CONTINUE;
}

update_status W_Game::PostUpdate(float dt)
{
	fbo->PostUpdate();
	return UPDATE_CONTINUE;
}

bool W_Game::CleanUp()
{
	fbo->DeleteBuffers();
	delete fbo;

	fbo = nullptr;

	return true;
}
