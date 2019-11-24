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
	scene["Game Objects"][obj_num]["Components"]["Mesh"]["UUID"] = UUID;
	scene["Game Objects"][obj_num]["Components"]["Mesh"]["DrawNormals"] = draw_normals;
}

const AABB& ComponentMesh::GetBoundingBox()
{
	return aabb;
}

void ComponentMesh::UpdateAABB()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose(res_mesh->vertex, res_mesh->num_vertex);

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
	for (std::vector<ComponentMesh*>::iterator iterator = App->scene_intro->meshes.begin(); iterator != App->scene_intro->meshes.end(); iterator++)
	{
		if ((*iterator)->my_GO->id == this->my_GO->id)
		{
			App->scene_intro->meshes.erase(iterator);
			break;
		}
	}


	if (res_mesh != nullptr)
	{
		res_mesh->loaded -= 1;
		if (res_mesh->loaded == 0)
		{
			res_mesh->ReleaseMemory();
		}
	}
}