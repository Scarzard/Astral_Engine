#include "Application.h"
#include "ModuleEngineUI.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "W_Game.h"
#include "W_Hierarchy.h"
#include "W_Console.h"
#include "W_Inspector.h"
#include "W_Configuration.h"

#include "mmgr/mmgr.h"




ModuleEngineUI::ModuleEngineUI( bool start_enabled) : Module(start_enabled)
{
}

ModuleEngineUI::~ModuleEngineUI()
{}

bool ModuleEngineUI::Init()
{
	IMGUI_CHECKVERSION();
	SDL_GL_MakeCurrent(App->window->window, App->renderer3D->context);
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	
	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Enable docking

	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init();
	//MEMORY LEAK HERE
	windows.push_back(new W_Game());
	windows.push_back(new W_Hierarchy());
	windows.push_back(new W_Console());
	windows.push_back(new W_Inspector());
	windows.push_back(new W_Configuration());
	
	return true;
}

bool ModuleEngineUI::Start()
{
	bool ret = true;
	std::list<Window*>::const_iterator tmp = windows.begin();
	while (tmp != windows.end())
	{
		ret = (*tmp)->Start();
		tmp++;
	}
	return ret;
}

bool ModuleEngineUI::Draw()
{
	bool ret = true;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	CreateMainMenuToolbar();

	std::list<Window*>::const_iterator tmp = windows.begin();
	while (tmp != windows.end())
	{
		ret = (*tmp)->Draw();
		tmp++;
	}

	//Render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
}


update_status ModuleEngineUI::PreUpdate(float dt)
{  
	update_status ret = UPDATE_CONTINUE;

	std::list<Window*>::const_iterator tmp = windows.begin();
	while (tmp != windows.end())
	{
		ret = (*tmp)->PreUpdate(dt);
		tmp++;
	}
	return ret;
}

update_status ModuleEngineUI::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	std::list<Window*>::const_iterator tmp = windows.begin();
	while (tmp != windows.end())
	{
		ret = (*tmp)->PostUpdate(dt);
		tmp++;
	}
	return ret;
}

bool ModuleEngineUI::CleanUp()
{
	bool ret = true;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}

void ModuleEngineUI::CreateMainMenuToolbar()
{

	bool opt_fullscreen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace_Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("My_Dock_Space");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	//Main toolbar
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{

			}
			if (ImGui::MenuItem("Open", "Ctrl+O")) 
			{

			}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("A");
				ImGui::MenuItem("B");
				ImGui::MenuItem("C");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");

					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Quit", "(Esc)"))
			{
				App->scene_intro->want_to_quit = UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Toggle demo window", NULL, &demo_cpp);

			
			/*ImGui::MenuItem("Toggle demo window", NULL, &show_demo_window);

			ImGui::MenuItem("Toggle MathGeoLib window", NULL, &show_mgl_window);

			ImGui::MenuItem("Toggle Number Generator Window", NULL, &test_rng_window);

			ImGui::MenuItem("Toggle Configuration window", NULL, &configuration_window);

			ImGui::MenuItem("Cosnole", NULL, &show_console);*/

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			ImGui::MenuItem("Cube", NULL);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
				App->OpenLink("https://github.com/Scarzard/Astral_Engine");

			if (ImGui::MenuItem("Download latest version"))
				App->OpenLink("https://github.com/Scarzard/Astral_Engine/releases");

			if (ImGui::MenuItem("Report a bug"))
				App->OpenLink("https://github.com/Scarzard/Astral_Engine/issues");

			ImGui::MenuItem("About", NULL, &about_window);

			ImGui::EndMenu();

		}
		if (about_window)
		{
			ImGui::OpenPopup("About");
			if (ImGui::BeginPopupModal("About"))
				ImGui::SetWindowSize(ImVec2(700, 510));
			{
				ImGui::Text("Placeholder Engine v0.1");
				ImGui::Separator();
				ImGui::Text("Best game engine in the world");
				ImGui::Separator();
				if (ImGui::Button("By Josep Lleal"))
					App->OpenLink("https://github.com/JosepLleal/");
				ImGui::SameLine();
				if (ImGui::Button("and Victor Chen"))
					App->OpenLink("https://github.com/Scarzard/");
				ImGui::Separator();
				//These have to redirect you into their respective websites
				ImGui::Text("3rd party libraries used");
				ImGui::BulletText("SDL");
				ImGui::BulletText("OpenGL");
				ImGui::BulletText("ImGui");
				ImGui::BulletText("Glew");
				ImGui::BulletText("PCG");
				ImGui::BulletText("MathGeoLib");
				ImGui::BulletText("Assimp");
				ImGui::BulletText("DevIL");
				
				ImGui::Separator();
				ImGui::TextWrapped("License\n\nMIT License\n\nCopyright(c) 2019 Josep Lleal and Victor Chen\n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
				if (ImGui::Button("Close"))
					about_window = false;
				ImGui::EndPopup();
			}
		}
	}
	ImGui::EndMainMenuBar();

	if (demo_cpp)
		ImGui::ShowDemoWindow();
}


