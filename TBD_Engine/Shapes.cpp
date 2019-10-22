#include "Shapes.h"
#include "Application.h"
#include "Module.h"

#include "par/par_shapes.h"
#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */


Shapes::Shapes() {}

Shapes::~Shapes() {}

void Shapes::LoadPrimitiveMesh(const par_shapes_mesh_s* m)
{
	GameObject* obj = App->scene_intro->CreateGameObject();

	// VERTEX ----------------
	obj->GetComponentMesh()->num_vertex = m->npoints;
	obj->GetComponentMesh()->vertex = new float3[obj->GetComponentMesh()->num_vertex];
	
	for (int i = 0; i < obj->GetComponentMesh()->num_vertex; i++)
	{
		int j = i * 3;
		obj->GetComponentMesh()->vertex[i].x = m->points[j];
		obj->GetComponentMesh()->vertex[i].y = m->points[j + 1];
		obj->GetComponentMesh()->vertex[i].z = m->points[j + 2];
	}

	// INDEX ------------
	obj->GetComponentMesh()->num_index = m->ntriangles * 3;
	obj->GetComponentMesh()->index = new uint[obj->GetComponentMesh()->num_index];

	for (int i = 0; i < obj->GetComponentMesh()->num_index; i++)
	{
		obj->GetComponentMesh()->index[i] = (uint)m->triangles[i];
	}

	// TEXTURE ----------------
	obj->GetComponentMesh()->num_tex_coords = m->npoints;
	obj->GetComponentMesh()->tex_coords = new float[obj->GetComponentMesh()->num_tex_coords * 2];

	//Copy the par_shapes texture coordinates
	for (int i = 0; i < obj->GetComponentMesh()->num_tex_coords * 2; ++i)
		obj->GetComponentMesh()->tex_coords[i] = m->tcoords[i];

	//Checkers texture to primitive
	obj->GetComponentMesh()->Texture = App->tex_loader->id_checkersTexture;

	//Generate the buffers 
	App->renderer3D->NewVertexBuffer(obj->GetComponentMesh()->vertex, obj->GetComponentMesh()->num_vertex, obj->GetComponentMesh()->id_vertex);
	App->renderer3D->NewIndexBuffer(obj->GetComponentMesh()->index, obj->GetComponentMesh()->num_index, obj->GetComponentMesh()->id_index);
	//Generate the buffer for texture coords
	App->renderer3D->NewTexBuffer(obj->GetComponentMesh()->tex_coords, obj->GetComponentMesh()->num_tex_coords, obj->GetComponentMesh()->id_tex_coords);

	App->LogInConsole("Created Primitive with %d vertices and %d indices.", obj->GetComponentMesh()->num_vertex, obj->GetComponentMesh()->num_index);

}

// CREATE SHAPES FUNCTIONS //

void Shapes::CreateSphere(float x, float y, float z, int slices, int stacks)
{
	//There has to be at least 3 slices and at least 3 stacks, otherwise it won't create the sphere :(

	obj = par_shapes_create_parametric_sphere(slices, stacks);

	position.x = x;
	position.y = y;
	position.z = z;

	par_shapes_translate(obj, position.x, position.y, position.z);

	LoadPrimitiveMesh(obj);

}

void Shapes::CreateTrefoil(float x, float y, float z, int slices, int stacks, int rad)
{
	//There has to be at least 3 slices and at least 3 stacks, otherwise it won't create the trefoil:(. And radius little than 3 better

	obj = par_shapes_create_trefoil_knot(slices, stacks, rad);
	position.x = x;
	position.y = y;
	position.z = z;

	par_shapes_translate(obj, position.x, position.y, position.z);

	LoadPrimitiveMesh(obj);
}

void Shapes::CreateTorus(float x, float y, float z, int slices, int stacks, int rad)
{
	obj = par_shapes_create_torus(slices, stacks, rad);
	position.x = x;
	position.y = y;
	position.z = z;

	par_shapes_translate(obj, position.x, position.y, position.z);

	LoadPrimitiveMesh(obj);
}

void Shapes::CreateCube(float x, float y, float z, float size)
{
	obj = par_shapes_create_cube();
	position.x = x;
	position.y = y;
	position.z = z;

	par_shapes_scale(obj, size, size, size);
	par_shapes_translate(obj, position.x, position.y, position.z);

	// may not work cause no UV's ?¿
	LoadPrimitiveMesh(obj);
}