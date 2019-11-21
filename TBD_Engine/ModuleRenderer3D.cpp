#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment(lib, "glew/libx86/glew32.lib")

#include "mmgr/mmgr.h"

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	App->LogInConsole("Creating 3D Renderer context");

	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	if(context == NULL)
	{
		App->LogInConsole("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->LogInConsole("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		// Initialize glew
		GLenum error = glewInit();
		App->LogInConsole("Using Glew %s", glewGetString(GLEW_VERSION));

		if (error != GL_NO_ERROR)
		{
			App->LogInConsole("Error initializing glew! %s\n"/*, glewGetErrorString(error)*/);
			ret = false;
		}


		//To detect our current hardware and driver capabilities
		App->LogInConsole("Vendor: %s", glGetString(GL_VENDOR));
		App->LogInConsole("Renderer: %s", glGetString(GL_RENDERER));
		App->LogInConsole("OpenGL version supported %s", glGetString(GL_VERSION));
		App->LogInConsole("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->LogInConsole("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->LogInConsole("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->LogInConsole("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		
		lights[0].Active(true);
	
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

	}

	// Projection matrix for
	//OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

bool ModuleRenderer3D::Start()
{

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	bool* update_camera = App->camera->GetProjectionUpdateFlag();
	if (*update_camera)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glLoadMatrixf((GLfloat*)App->camera->GetOpenGLProjection());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		*update_camera = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetOpenGLView());

	// light 0 on cam pos
//lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{

	App->scene_intro->Draw();

	//Draw UI
	App->gui->Draw();

	//LAST THING TO DO IN POSTUPDATE
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->LogInConsole("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	ComponentCamera* tmp = App->camera->GetActiveCamera();
	if (tmp != nullptr)
		tmp->has_transformed = true;
	else
		SDL_assert(false);
}

void ModuleRenderer3D::NewVertexBuffer(float3 * vertex, uint &size, uint &id_vertex)
{
	glGenBuffers(1, (GLuint*) &(id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * size, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::NewIndexBuffer(uint *index, uint &size, uint &id_index)
{
	glGenBuffers(1, (GLuint*) &(id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * size, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::NewTexBuffer(float * tex_coords, uint & num_tex_coords, uint & id_tex_coords)
{
	glGenBuffers(1, (GLuint*) &(id_tex_coords)); 
	glBindBuffer(GL_ARRAY_BUFFER, id_tex_coords); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_tex_coords*2, tex_coords, GL_STATIC_DRAW); 
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
}

void ModuleRenderer3D::Draw(GameObject* m) const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if (m->GetComponentTexture() != nullptr)
	{
		if (m->GetComponentTexture()->Checers_texture == true)
			glBindTexture(GL_TEXTURE_2D, App->tex_loader->CheckersTexture.id);
		else
			glBindTexture(GL_TEXTURE_2D, m->GetComponentTexture()->texture.id);


	}

	if (m->GetComponentMesh() != nullptr)
	{
		ComponentMesh* mesh = m->GetComponentMesh();

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_tex_coords);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
		glDrawElements(GL_TRIANGLES, mesh->num_index, GL_UNSIGNED_INT, nullptr);

		if (mesh->draw_normals)
		{
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);

			for (int j = 0; j < mesh->num_normals; ++j)
			{
				glVertex3f(mesh->face_center[j].x, mesh->face_center[j].y, mesh->face_center[j].z);
				glVertex3f(mesh->face_center[j].x + mesh->face_normal[j].x, mesh->face_center[j].y + mesh->face_normal[j].y, mesh->face_center[j].z + mesh->face_normal[j].z);
			}

			glColor3f(1, 1, 1);
			glEnd();
		}

	}

	if (m->GetComponentCamera() != nullptr)
	{
		ComponentCamera* camera = m->GetComponentCamera();

		camera->Update();
	}
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

		
}