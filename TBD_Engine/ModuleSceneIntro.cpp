#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "par/par_shapes.h"
#include "Math.h"

#include "mmgr/mmgr.h"



ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
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

	object = new Shapes();
	object->CreateCube(1, 1, -5);

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
	

	return want_to_quit;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	//PLANE -----------------------------
	glLineWidth(2.0f);

	glBegin(GL_LINES);
	glColor3ub(255, 255, 255);
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
	//DrawCube_36v(-5, 0, -5, 2, 255, 255, 0);
	//--------------------------------------------------------

	//Rendering cube with 8 vertices -----------------
	/*DrawCube_8v(1, 1, -10, 1, 255, 0, 0);
	DrawCube_8v(0, 1, -10, 1, 0, 0, 255);
	DrawCube_8v(-1, 1, -10, 1, 255, 0, 0);*/

	//DrawCube_8v(5,5,5,50,255,255,255);
	//--------------------------------------------------------------

	glColor3ub(255, 255, 255);
	object->RenderShape();

	/*glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);

	glColor3ub(255, 255, 255);

	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);

	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);

	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);

	glVertex3f(1.f, 0.f, -1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glVertex3f(1.f, 1.f, 0.f);

	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 0.f, -1.f);

	glVertex3f(0.f, 1.f, -1.f);
	glVertex3f(0.f, 0.f, -1.f);
	glVertex3f(0.f, 1.f, 0.f);

	glVertex3f(0.f, 1.f, -1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glVertex3f(0.f, 0.f, -1.f);

	glVertex3f(1.f, 0.f, -1.f);
	glVertex3f(0.f, 0.f, -1.f);
	glVertex3f(1.f, 1.f, -1.f);

	glEnd();*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawCube_8v(float x, float y, float z, float size, uint red, uint green, uint blue)
{

	float No_dup_vertices[]
	{			
		x + size,	y + size,	z + 0.f,	//0
		x + 0.f,	y + size,	z + 0.f,	//1
		x + 0.f,	y + 0.f,	z + 0.f,	//2
		x + size,	y + 0.f,	z + 0.f,	//3
		x + size,	y + 0.f,	z + size,	//4
		x + size,	y + size,	z + size,	//5
		x + 0.f,	y + size,	z + size,	//6
		x + 0.f,	y + 0.f,	z + size,	//7	
	};

	GLuint indices[] = { 0,1,2, 2,3,0,   // 36 of indices
					 0,3,4, 4,5,0,
					 0,5,6, 6,1,0,
					 1,6,7, 7,2,1,
					 7,4,3, 3,2,7,
					 4,7,6, 6,5,4 };


	glGenBuffers(1, (uint*) &(id_no_dup_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_no_dup_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(No_dup_vertices), No_dup_vertices, GL_STATIC_DRAW);


	glGenBuffers(1, (uint*) &(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glColor3ub(red, green, blue);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_no_dup_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleSceneIntro::DrawCube_36v(float x, float y, float z, float size, uint red, uint green, uint blue)
{

	float vertices[] =
	{
		x + 0.f,	 y + size,		z + 0.f,
		x + 0.f,	 y + 0.f,		z + 0.f,
		x + 0.f,	 y + 0.f,		z + size,
		x + 0.f,	 y + size,		z + 0.f,
		x + 0.f,	 y + 0.f,		z + size,
		x + 0.f,	 y + size,		z + size,
		x + 0.f,	 y + size,		z + size,
		x + 0.f,	 y + 0.f,		z + size,
		x + size,	 y +  0.f,		z + size,
		x + 0.f,	 y + size,		z + size,
		x + size,	 y +  0.f,		z + size,
		x + size,	 y +  size,		z + size,
		x + 0.f,	 y + size,		z + 0.f,
		x + 0.f,	 y + size,		z + size,
		x + size,	 y +  size,		z + size,
		x + 0.f,	 y + size,		z + 0.f,
		x + size,	 y +  size,		z + size,
		x + size,	 y +  size,		z + 0.f,
		x + size,	 y +  0.f,		z + size,
		x + size,	 y +  0.f,		z + 0.f,
		x + size,	 y +  size,		z + 0.f,
		x + size,	 y +  size,		z + size,
		x + size,	 y +  0.f,		z + size,
		x + size,	 y +  size,		z + 0.f,
		x + size,	 y +  0.f,		z + 0.f,
		x + 0.f,	 y + 0.f,		z + 0.f,
		x + 0.f,	 y + size,		z + 0.f,
		x + size,	 y +  size,		z + 0.f,
		x + size,	 y +  0.f,		z + 0.f,
		x + 0.f,	 y + size,		z + 0.f,
		x + size,	 y +  0.f,		z + size,
		x + 0.f,	 y + 0.f,		z + size,
		x + 0.f,	 y + 0.f,		z + 0.f,
		x + size,	 y +  0.f,		z + 0.f,
		x + size,	 y +  0.f,		z + size,
		x + 0.f,	 y + 0.f,		z + 0.f
	};

	
	glGenBuffers(1, (GLuint*) &(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, vertices, GL_STATIC_DRAW);

	glColor3ub(red, green, blue);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);

	
}


