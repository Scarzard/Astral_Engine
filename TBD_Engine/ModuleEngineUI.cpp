#include "Application.h"
#include "Globals.h"
#include "ModuleEngineUI.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include <string>


ModuleEngineUI::ModuleEngineUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEngineUI::~ModuleEngineUI()
{}

bool ModuleEngineUI::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	return true;
}

bool ModuleEngineUI::Start()
{
	bool ret = true;

	return ret;
}

update_status ModuleEngineUI::PreUpdate(float dt)
{  
	// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEngineUI::Update(float dt)
{
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

	//Main toolbar
	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File", true)) {

			if (ImGui::MenuItem("Exit", "(Esc)", false, true)) {

				return update_status::UPDATE_STOP;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools", true)) {

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (main_window)
	{
		ImGui::Begin("Main window");
		if (ImGui::Button("Open Demo window 1"))
			show_demo_window = true;
		if (ImGui::Button("Open Demo window 2"))
			show_another_window = true;
		if (ImGui::Button("Open RNG window"))
			test_rng_window = true;
		ImGui::End();
	}
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		ImGui::ShowDemoWindow(&show_demo_window);
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	if (test_rng_window)
	{
		ImGui::Begin("Random Number Generator Window", &test_rng_window);
		ImGui::Separator();

		if (ImGui::Button("Float between 0 and 1.0"))
			rn1 = ldexp(pcg32_random_r(&rng), -32);
		ImGui::SameLine();
		ImGui::Text(std::to_string(rn1).c_str());

		if (ImGui::Button("0-10"))
			rn2 = pcg32_boundedrand_r(&rng_bounded, 10);
		ImGui::SameLine();
		ImGui::Text("Randomized number = %d", rn2);

		if (ImGui::Button("Totally random"))
			rn3 = pcg32_random_r(&rng_bounded2);
		ImGui::SameLine();
		ImGui::Text("Randomized number = %d", rn3);


		ImGui::End();
		
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEngineUI::PostUpdate(float dt)
{

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleEngineUI::CleanUp()
{
	bool ret = true;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}