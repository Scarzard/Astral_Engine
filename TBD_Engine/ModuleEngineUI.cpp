#include "Application.h"
#include "ModuleEngineUI.h"
#include "ModuleRenderer3D.h"
#include "SceneGame.h"


ModuleEngineUI::ModuleEngineUI(Application* app, bool start_enabled) : Module(app, start_enabled)
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
	
	windows.push_back(new SceneGame(App));
	
	return true;
}

bool ModuleEngineUI::Start()
{
	bool ret = true;
	std::list<Module*>::const_iterator tmp = windows.begin();
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

	std::list<Module*>::const_iterator tmp = windows.begin();
	while (tmp != windows.end())
	{
		ret = (*tmp)->Draw();
		tmp++;
	}
	
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow();
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		ImGui::PopStyleVar();
	}

	//render

	// Rendering function called in rednerer3D 
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
}


update_status ModuleEngineUI::PreUpdate(float dt)
{  
	update_status ret = UPDATE_CONTINUE;

	std::list<Module*>::const_iterator tmp = windows.begin();
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

	std::list<Module*>::const_iterator tmp = windows.begin();
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
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	//Main toolbar
	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File", true)) {

			if (ImGui::MenuItem("Exit", "(Esc)", false, true)) {

			/*	return update_status::UPDATE_STOP;*/
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Toggle demo window", NULL, &show_demo_window);
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
				App->OpenLink("https://github.com/Scarzard/Placeholder_Engine/wiki");

			if (ImGui::MenuItem("Download latest version"))
				App->OpenLink("https://github.com/Scarzard/Placeholder_Engine/releases");

			if (ImGui::MenuItem("Report a bug"))
				App->OpenLink("https://github.com/Scarzard/Placeholder_Engine/issues");

			//ImGui::MenuItem("About", NULL, &about_window);

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
}
