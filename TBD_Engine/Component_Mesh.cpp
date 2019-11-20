#include "Component_Mesh.h"
#include "Application.h"
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

const AABB& ComponentMesh::GetBoundingBox()
{
	return aabb;
}

void ComponentMesh::CleanUp()
{

	for (std::vector<ComponentMesh*>::iterator iterator = App->scene_intro->static_meshes.begin(); iterator != App->scene_intro->static_meshes.end(); iterator++)
	{
		if ((*iterator)->my_GO->id == this->my_GO->id)
		{
			App->scene_intro->static_meshes.erase(iterator);
			break;
		}
	}

	glDeleteBuffers(1, (GLuint*)&id_index);
	glDeleteBuffers(1, (GLuint*)&id_vertex);
	glDeleteBuffers(1, (GLuint*)&id_tex_coords);

	if (index != nullptr)
	{
		delete[] index;
		index = nullptr;
	}

	if (vertex != nullptr)
	{
		delete[] vertex;
		vertex = nullptr;
	}

	if (tex_coords != nullptr)
	{
		delete[] tex_coords;
		tex_coords = nullptr;
	}

	if (face_center != nullptr)
	{
		delete[] face_center;
		face_center = nullptr;
	}

	if (face_normal != nullptr)
	{
		delete[] face_normal;
		face_normal = nullptr;
	}
}