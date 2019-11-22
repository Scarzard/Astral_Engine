#include "Component_Mesh.h"
#include "Application.h"
#include "Color.h"
#include "Component_Transform.h"
#include "ModuleSceneIntro.h"
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

void ComponentMesh::Save(uint obj_num, nlohmann::json &scene)
{
	scene["Game Objects"][obj_num]["Components"]["Mesh"]["DrawNormals"] = draw_normals;
}

const AABB& ComponentMesh::GetBoundingBox()
{
	return aabb;
}

void ComponentMesh::UpdateAABB()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose(vertex, num_vertex);

}

void ComponentMesh::UpdateGlobalAABB()
{
	OBB obb = aabb;
	obb.Transform(my_GO->GetComponentTransform()->GetGlobalTransform());

	global_aabb.SetNegativeInfinity();
	global_aabb.Enclose(obb);
}

void ComponentMesh::DrawAABB()
{
	int num_v = aabb.NumVerticesInEdgeList();
	math::float3* vertices = new math::float3[num_v];
	aabb.ToEdgeList((float3*)vertices);

	glBegin(GL_LINES);
	glColor4f(Green.r, Green.g, Green.b, Green.a);

	for (uint i = 0; i < aabb.NumVerticesInEdgeList(); i++)
	{
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}

	delete[] vertices;

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
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