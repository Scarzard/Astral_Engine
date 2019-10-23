#include "Component_Mesh.h"
#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "mmgr/mmgr.h"

ComponentMesh::ComponentMesh(GameObject* GO) : Component(Component::ComponentType::Mesh, GO)
{
	
}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::CleanUp()
{
	glDeleteBuffers(1, (GLuint*)&id_index);
	glDeleteBuffers(1, (GLuint*)&id_vertex);
	glDeleteBuffers(1, (GLuint*)&id_tex_coords);

	if (index)
	{
		delete[] index;
		index = nullptr;
	}

	if (vertex)
	{
		delete[] vertex;
		vertex = nullptr;
	}

	if (tex_coords)
	{
		delete[] tex_coords;
		tex_coords = nullptr;
	}
}