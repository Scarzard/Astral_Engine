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

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
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


		glViewport(0,0,App->window->width, App->window->height);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

bool ModuleRenderer3D::Start()
{
	float size = 1.0f;

	float vertices[108] =
	{
		0.f, size, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, -size,
		0.f, size, 0.f,
		0.f, 0.f, -size,
		0.f, size, -size,
		0.f, size, -size,
		0.f, 0.f, -size,
		size, 0.f, -size,
		0.f, size, -size,
		size, 0.f, -size,
		size, size, -size,
		0.f, size, 0.f,
		0.f, size, -size,
		size, size, -size,
		0.f, size, 0.f,
		size, size, -size,
		size, size, 0.f,
		size, 0.f, -size,
		size, 0.f, 0.f,
		size, size, 0.f,
		size, size, -size,
		size, 0.f, -size,
		size, size, 0.f,
		size, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, size, 0.f,
		size, size, 0.f,
		size, 0.f, 0.f,
		0.f, size, 0.f,
		size, 0.f, -size,
		0.f, 0.f, -size,
		0.f, 0.f, 0.f,
		size, 0.f, 0.f,
		size, 0.f, -size,
		0.f, 0.f, 0.f
	};

	float No_dup_vertices[24]
	{						//indices
		0.f,size,0.f,		//0
		0.f,0.f,0.f,		//1
		0.f,0.f,-size,		//2		
		0.f, size, -size,	//3
		size,size,0.f,		//4
		size,0.f,0.f,		//5
		size,0.f,-size,		//6
		size, size, -size,	//7
	};

	uint indices[36] = { 0,1,2, 2,3,0, 0,3,4, 4,5,0, 0,5,6, 6,1,0, 1,6,7, 7,2,1, 7,4,3, 3,2,7, 4,7,6, 6,5,4, };

	//BUFFERS

	glGenBuffers(1, (GLuint*) &(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 35 * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(id_no_dup_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_no_dup_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, No_dup_vertices, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*) &(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{

	Color c = App->camera->background;
	glClearColor(c.r, c.g, c.b, c.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//PLANE -----------------------------
	glLineWidth(2.0f);

	glBegin(GL_LINES);
	glColor3ub(0, 255, 0);
	for (float i = -10; i <= 10; ++i)
	{
		glVertex3f(i, 0.f, 0.f);
		glVertex3f(i, 0, 10.f);

		glVertex3f(0.f, 0.f, i);
		glVertex3f(10.f, 0, i);

		glVertex3f(i, 0.f, 0.f);
		glVertex3f(i, 0, -10.f);

		glVertex3f(0.f, 0.f, i);
		glVertex3f(-10.f, 0, i);
	}
	glEnd();

	// AXIS ------------------------
	glLineWidth(4.0f);

	glBegin(GL_LINES);
	//Y
	glColor3ub(0, 255, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glEnd();

	glBegin(GL_LINES);
	//X
	glColor3ub(255, 0, 0);
	glVertex3f(0.f, 0.001f, 0.f);
	glVertex3f(1.f, 0.001f, 0.f);
	glEnd();

	glBegin(GL_LINES);
	//Z
	glColor3ub(0, 0, 255);
	glVertex3f(0.f, 0.001f, 0.f);
	glVertex3f(0.f, 0.001f, 1.f);
	glEnd();
	//---------------------------------

	//Render

	//Rendering cube with 36 vertices (not optimal)----------
	glColor3ub(255, 255, 255);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);
	//--------------------------------------------------------

	//Rendering cube with 8 vertices (not working)-----------------
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, id_no_dup_vertices);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, &id_indices);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//--------------------------------------------------------------

	//Debug Render

	//UI Render
	App->gui->UI_Render();

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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
