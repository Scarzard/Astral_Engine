#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleEngineUI.h"
#include "ModuleWindow.h"
#include "ModuleResources.h"
#include "mmgr/mmgr.h"



#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	App->LogInConsole("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->LogInConsole("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= App->window->size;
	mouse_y /= App->window->size;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		//ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / App->window->size;
			mouse_y = e.motion.y / App->window->size;

			mouse_x_motion = e.motion.xrel / App->window->size;
			mouse_y_motion = e.motion.yrel / App->window->size;
			break;

			case SDL_DROPFILE:     

				DroppedFile = e.drop.file;

				if (App->GetFileExtension(DroppedFile) == "FBX" || App->GetFileExtension(DroppedFile) == "fbx")
				{
					App->mesh_loader->LoadFile(DroppedFile);
					App->LogInConsole("FBX dropped with root: %s", DroppedFile);
				}
				else if (App->GetFileExtension(DroppedFile) == "png" || App->GetFileExtension(DroppedFile) == "dds" || App->GetFileExtension(DroppedFile) == "PNG" || App->GetFileExtension(DroppedFile) == "DDS" || App->GetFileExtension(DroppedFile) == "jpg" || App->GetFileExtension(DroppedFile) == "tga")
				{
					if (App->gui->ins_window->selected_GO != nullptr)
					{
						App->gui->ins_window->selected_GO->GetComponentTexture()->res_texture = (ResourceTexture*)App->resources->Get(App->resources->GetNewFile(DroppedFile));
						App->gui->ins_window->selected_GO->GetComponentTexture()->res_texture->UpdateNumReference();
						App->LogInConsole("Texture dropped with root: %s", DroppedFile);
					}
					else
					{
						App->LogInConsole("ERROR: Select a GameObject before dropping texture");
					}
					
				}
				else
				{
					App->LogInConsole("File dropped extension not supported! Supported extensions: .fbx, .png, .dds");
				}
				
				
				

				SDL_free((char*)DroppedFile);
				break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->LogInConsole("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}


