#include "Component_Mesh.h"
#include "Application.h"
#include "Color.h"
#include "Component_Transform.h"
#include "ModuleSceneIntro.h"
#include "ModuleResources.h"
#include "GameObject.h"

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

void ComponentMesh::AddBone(ComponentBone* bone)
{
	for (int i = 0; i < bones.size(); i++)
		if (bones[i] == bone)
			return;

	bones.push_back(bone);
}

void ComponentMesh::UpdateDefMesh()
{
	if (deformable_mesh == nullptr)
	{

		deformable_mesh = new ResourceMesh(App->GetRandomUUID());

		deformable_mesh->num_index = res_mesh->num_index;
		deformable_mesh->index = new uint[deformable_mesh->num_index];
		memcpy(deformable_mesh->index, res_mesh->index, deformable_mesh->num_index * sizeof(uint));

		deformable_mesh->num_vertex = res_mesh->num_vertex;
		deformable_mesh->vertex = new float3[deformable_mesh->num_vertex];

		deformable_mesh->num_normals = res_mesh->num_normals;
		deformable_mesh->face_center = new float3[deformable_mesh->num_normals];

		deformable_mesh->num_normals = res_mesh->num_normals;
		deformable_mesh->face_normal = new float3[deformable_mesh->num_normals];

		App->renderer3D->NewVertexBuffer(deformable_mesh->vertex, deformable_mesh->num_vertex, deformable_mesh->id_vertex);
		
	}

	// -------------------- RESET DEFORMABLE MESH --------------------------
	for (int i = 0; i < deformable_mesh->num_vertex; i++)
		deformable_mesh->vertex[i] = float3(0, 0, 0);

	for (int i = 0; i < deformable_mesh->num_normals; i++)
	{
		deformable_mesh->face_normal[i] = float3(0, 0, 0);
		deformable_mesh->face_center[i] = float3(0, 0, 0);
	}
	//-----------------------------------------------------------------------

	for (std::vector<ComponentBone*>::iterator it = bones.begin(); it != bones.end(); it++)
	{
		ResourceBone* r_bone = (*it)->res_bone;

		float4x4 mat = (*it)->GetSkeletonTransform();
		mat = my_GO->GetComponentTransform()->GetTransform().Inverted() * mat;
		mat = mat * r_bone->matrix;

		for (uint i = 0; i < r_bone->NumWeights; i++)
		{
			uint index = r_bone->index_weight[i];
			float3 tmp = res_mesh->vertex[index];
			float3 _vertex = mat.TransformPos(tmp);

			deformable_mesh->vertex[index].x += _vertex.x * r_bone->weight[i];
			deformable_mesh->vertex[index].y += _vertex.y * r_bone->weight[i];
			deformable_mesh->vertex[index].z += _vertex.z * r_bone->weight[i];

			if (res_mesh->num_normals > 0)
			{
				_vertex = mat.TransformPos(res_mesh->face_normal[index]);
				deformable_mesh->face_normal[index].x += _vertex.x * r_bone->weight[i];
				deformable_mesh->face_normal[index].y += _vertex.y * r_bone->weight[i];
				deformable_mesh->face_normal[index].z += _vertex.z * r_bone->weight[i];

				_vertex = mat.TransformPos(res_mesh->face_center[index]);
				deformable_mesh->face_center[index].x += _vertex.x * r_bone->weight[i];
				deformable_mesh->face_center[index].y += _vertex.y * r_bone->weight[i];
				deformable_mesh->face_center[index].z += _vertex.z * r_bone->weight[i];
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