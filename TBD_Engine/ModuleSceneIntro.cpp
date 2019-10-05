#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Math.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->LogInConsole("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	uint my_id = 0;
	glGenBuffers (1, (GLuint*) &(my_id));
	glBindBuffer (GL_ARRAY_BUFFER, my_id);
	glBufferData (GL_ARRAY_BUFFER, sizeof(float)*num_vertices*3, vertices, GL_STATIC_DRAW);

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);


	


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->LogInConsole("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	
	

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	
	PlaneP p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);

	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(2.f, 0.f, 0.f);
	glVertex3f(0.f, 2.f, 0.f);	
	

	glEnd();



	return UPDATE_CONTINUE;
}



