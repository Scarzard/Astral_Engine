#include "ResourceMesh.h"
#include "ModuleResources.h"
#include "MeshLoader.h"
#include "Application.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "mmgr/mmgr.h"

bool ResourceMesh::LoadInMemory()
{
	return App->mesh_loader->Load(this);
}

void ResourceMesh::ReleaseMemory()
{
	std::map<uint, Resource*>::const_iterator it = App->resources->resources.find(this->res_UUID);

	if(it != App->resources->resources.end())
		App->resources->resources.erase(it);

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


	delete this;
}