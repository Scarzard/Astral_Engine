#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include <string>

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Start()
{
	bool ret = true;

	min_ = max_ = min = max = 0;
	spheresIntersecting = trianglesIntersecting = AABBIntersecting = false;

	sphere_1 = Sphere({ 0.0f, 0.0f, 0.0f }, 1.0f);
	sphere_2 = Sphere({ 5.0f, 0.0f, 0.0f }, 1.0f);

	triangle_1 = Triangle(float3(7.0f, 0.0f, 0.0f), float3(0.0f, 7.0f, 0.0f), float3(0.0f, 0.0f, 7.0f));
	triangle_2 = Triangle(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f));

	aabb_1 = AABB(float3(-2.0f, -2.0f, -2.0f), float3(0.0f, 0.0f, 0.0f));
	aabb_2 = AABB(float3(0.0f, 0.0f, 0.0f), float3(2.0f, 2.0f, 2.0f));

	return ret;
}

update_status ModuleEditor::Update(float dt)
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

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Toggle demo window"))
				show_demo_window = !show_demo_window;

			if (ImGui::MenuItem("Toggle MathGeoLib window"))
				show_mgl_window = !show_mgl_window;

			if (ImGui::MenuItem("Toggle Number Generator Window"))
				test_rng_window = !test_rng_window;

			if (ImGui::MenuItem("Toggle Configuration window"))
				configuration_window = !configuration_window;

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


			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

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
		ImGui::Checkbox("Another Window", &show_mgl_window);

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
	if (show_mgl_window)
	{
		ImGui::Begin("Another Window", &show_mgl_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_mgl_window = false;


		// MGL testers
		float x_ = 0.0f;
		ImGui::SliderFloat("Move sphere_1 x pos", &x_, -10.f, 10.0f);
		sphere_1 = Sphere({ x_, 0.0f, 0.0f }, 1.0f);

		int trianglePos = 0;
		ImGui::SliderInt("Move triangle_2 pos", &trianglePos, -10, 10);
		triangle_2 = Triangle(float3((float)trianglePos, 0.0f, 0.0f), float3(0.0f, (float)trianglePos, 0.0f), float3(0.0f, 0.0f, (float)trianglePos));

		int AABBpos = 0;
		ImGui::SliderInt("Move aabb_1 pos", &AABBpos, -3, 3);
		aabb_1 = AABB(float3((float)AABBpos, (float)AABBpos, (float)AABBpos), float3((float)AABBpos, (float)AABBpos, (float)AABBpos));

		if (sphere_1.Intersects(sphere_2))
		{
			spheresIntersecting = true;
		}
		else spheresIntersecting = false;

		if (triangle_2.Intersects(triangle_1))
		{
			trianglesIntersecting = true;
		}
		else trianglesIntersecting = false;

		if (aabb_1.Intersects(aabb_2))
		{
			AABBIntersecting = true;
		}
		else AABBIntersecting = false;
		ImGui::Separator();
		ImGui::Text("Are spheres intersecting ? %d", spheresIntersecting);
		ImGui::Separator();
		ImGui::Text("Are triangles intersecting ? %d", trianglesIntersecting);
		ImGui::Separator();
		ImGui::Text("Are AABBs intersecting ? %d", AABBIntersecting);



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
		ImGui::Separator();

		if (ImGui::Button("0-10"))
			rn2 = pcg32_boundedrand_r(&rng_bounded, 10);
		ImGui::SameLine();
		ImGui::Text("Randomized number = %d", rn2);
		ImGui::Separator();

		if (ImGui::Button("Totally random"))
			rn3 = pcg32_random_r(&rng_bounded);
		ImGui::SameLine();
		ImGui::Text("Randomized number = %d", rn3);
		ImGui::Separator();

		//Random number within the intervals that you set yourself
		ImGui::InputInt("Minimum limit", &min_);
		ImGui::InputInt("Maximum limit ", &max_);


		ImGui::SliderInt("min", &min, min_, max_);
		ImGui::SameLine();
		ImGui::Text(" = %d", min);

		ImGui::SliderInt("max", &max, min_, max_);
		ImGui::SameLine();
		ImGui::Text(" = %d", max);

		if (ImGui::Button("Random number between the set interval"))
			interval_random = pcg32i_boundedrand_r(&rng_bounded2, (max - min) + 1);


		ImGui::SameLine();
		ImGui::Text("Randomized number = %d", (interval_random));


		ImGui::End();

	}

	if (configuration_window)
	{
		ImGui::Begin("Application Configuration Window", &configuration_window);
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::InputText("Application name", TITLE, 20);
			ImGui::InputText("Organization name", ORGANIZATION, 40);
			//Framerate Histograms
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::SliderFloat("Brightness", &brightness_slider, 0.0f, 1.0f);
			App->window->SetBrightness(brightness_slider);

			ImGui::SliderInt("Width", &width_slider, 600, 1920);
			App->window->SetWidth(width_slider);

			ImGui::SliderInt("Height", &height_slider, 400, 1080);
			App->window->SetHeigth(height_slider);


			ImGui::Checkbox("Fullscreen", &App->window->fullscreen);
			App->window->SetFullscreen(App->window->fullscreen);
			ImGui::SameLine();
		
			ImGui::Checkbox("Resizable", &App->window->resizable);
			App->window->SetResizable(App->window->resizable);

			ImGui::Checkbox("Borderless", &App->window->borderless);
			App->window->SetBorderless(App->window->borderless);
			ImGui::SameLine();
			ImGui::Checkbox("Fullscreen Desktop", &App->window->fullscreen_desktop);
			App->window->SetFullScreenDesktop(App->window->fullscreen_desktop);



		}
		if (ImGui::CollapsingHeader("Input"))
		{

		}

		if (ImGui::CollapsingHeader("Hardware"))
		{

		}

		ImGui::End();
	}

	return UPDATE_CONTINUE;
}


bool ModuleEditor::CleanUp()
{
	bool ret = true;


	return ret;
}