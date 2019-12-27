#include "Component_Mesh.h"
#include "Application.h"
#include "Color.h"
#include "Component_Transform.h"
#include "ModuleSceneIntro.h"
#include "ModuleResources.h"

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
	scene[my_GO->name]["Components"]["Mesh"]["UUID"] = std::to_string(UUID);
	scene[my_GO->name]["Components"]["Mesh"]["DrawNormals"] = std::to_string(draw_normals);
	scene[my_GO->name]["Components"]["Mesh"]["ResourceName"] = App->GetNameFromPath(res_mesh->exported_file);
}

void ComponentMesh::AttachSkeleton(GameObject* go)
{
	tmp_id = go->id;

	deformable_mesh = (ResourceMesh*)App->resources->NewResource(Resource::RES_TYPE::MESH);
	AttachBone(go);
}

void ComponentMesh::AttachSkeleton()
{
	for (int i = 0; i < my_GO->children.size(); i++)
	{
		AttachSkeleton(my_GO);
	}
}

void ComponentMesh::AttachBone(GameObject* go)
{
	ComponentBone* tmp = go->GetComponentBone();

	if (tmp != nullptr)
		bones.push_back(tmp);

	for(int i = 0; i < my_GO->children.size(); i++)
	{
		AttachBone(go);
	}
}

void ComponentMesh::UpdateMesh()
{
	for (std::vector<ComponentBone*>::iterator it = bones.begin(); it != bones.end(); ++it)
	{
		ResourceBone* rBone = (ResourceBone*)(*it)->GetType();

		float4x4 mat = (*it)->my_GO->GetComponentTransform()->GetTransform();
		mat = my_GO->GetComponentTransform()->GetTransform().Inverted() * mat;
		mat = mat * rBone->matrix;

		for (uint i = 0; i < rBone->NumWeights; i++)
		{
			uint index = rBone->index_weight[i];
			float3 tmp = res_mesh->vertex[index];
			float3 extra = mat.TransformPos(tmp);

			deformable_mesh->vertex[index].x += extra.x * rBone->index_weight[i];
			deformable_mesh->vertex[index].y += extra.y * rBone->index_weight[i];
			deformable_mesh->vertex[index].z += extra.z * rBone->index_weight[i];

			if (res_mesh->num_normals > 0)
			{
				extra = mat.TransformPos(res_mesh->face_normal[index]);
				deformable_mesh->face_normal[index].x += extra.x * rBone->index_weight[i];
				deformable_mesh->face_normal[index].y += extra.y * rBone->index_weight[i];
				deformable_mesh->face_normal[index].z += extra.z * rBone->index_weight[i];
			}
		}
	}
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
	}
}