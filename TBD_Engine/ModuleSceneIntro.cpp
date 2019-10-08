#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

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

	return UPDATE_CONTINUE;
}



