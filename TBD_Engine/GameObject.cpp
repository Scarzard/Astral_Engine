#include "GameObject.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Color.h"

#include "mmgr/mmgr.h"

GameObject::GameObject(std::string name)
{
	this->name = name;
	unactive_name = name + " (not active)";
	this->active = true;

	if(id!=0)
	{
		CreateComponent(Component::ComponentType::Transform);
		CreateComponent(Component::ComponentType::Mesh);
		CreateComponent(Component::ComponentType::Texture);
		CreateBoundingBox(this->GetComponentMesh());
	}
}

GameObject::~GameObject()
{
}

Component* GameObject::CreateComponent(Component::ComponentType type)
{
	
	Component* component = nullptr;

	switch (type)
	{
	case Component::ComponentType::Transform:
		component = new ComponentTransform(this);
		break;
	case Component::ComponentType::Mesh:
		component = new ComponentMesh(this);
		break;
	case Component::ComponentType::Texture:
		component = new ComponentTexture(this);
		break;

	}

	if (component)
		components.push_back(component);

	return component;
}

ComponentMesh* GameObject::GetComponentMesh()
{
	Component* mesh = nullptr;
	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); iterator++)
	{
		if ((*iterator)->type == Component::ComponentType::Mesh)
		{
			return (ComponentMesh*)*iterator;
		}
	}

	return (ComponentMesh*)mesh;
}

ComponentTransform* GameObject::GetComponentTransform()
{
	Component* transform = nullptr;
	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); iterator++)
	{
		if ((*iterator)->type == Component::ComponentType::Transform)
		{
			return (ComponentTransform*)*iterator;
		}
	}

	return (ComponentTransform*)transform;
}

ComponentTexture* GameObject::GetComponentTexture()
{
	Component* texture = nullptr;
	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); iterator++)
	{
		if ((*iterator)->type == Component::ComponentType::Texture)
		{
			return (ComponentTexture*)*iterator;
		}
	}

	return (ComponentTexture*)texture;
}

void GameObject::Update(float dt)
{
	//update name when GO change state
	std::string str = this->name + " (not active)";
	if(unactive_name.compare(str) != 0)
		unactive_name = name + " (not active)";

	if (this->GetComponentTransform()->has_transformed)
		TransformGlobal(this);

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->Update(dt);
	}
	
	if(active)
		if (aabb != nullptr)
		{
			ComponentTransform* transform = this->GetComponentTransform();
			if (transform != nullptr)
			{
				bounding_box = *aabb;
				bounding_box.TransformAsAABB(transform->GetGlobalTransform());
			}
		}
	RenderBoundingBox();
}

void GameObject::Enable()
{
	if(this->active == false)
		this->active = true;

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->Enable();
	}
}

void GameObject::Disable()
{
	if (this->active == true)
		this->active = false;

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->Disable();
	}
}

void GameObject::DeleteGO(GameObject* GO)
{
	//first delete its childrens (if it has)
	if (GO->children.size() > 0)
	{
		for (std::vector<GameObject*>::iterator it = GO->children.begin(); it != GO->children.end(); ++it)
		{
			DeleteGO(*it);
		}

		GO->children.clear();
	}

	delete GO;
	GO = nullptr;
}

void GameObject::SetChild(GameObject* GO)
{
	if (GO->parent != nullptr)
		GO->parent->RemoveChild(GO);

	GO->parent = this;
	children.push_back(GO);

	//ComponentTransform* transform = GO->GetComponentTransform();
	//transform->SetGlobalTransform(GO->GetComponentTransform()->GetGlobalTransform());
}

void GameObject::RemoveChild(GameObject* GO)
{
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		if ((*it)->id == GO->id)
		{
			children.erase(it);
			break;
		}
	}
}

void GameObject::CreateBoundingBox(ComponentMesh * tmp)
{
	if (aabb == nullptr)
	{
		aabb = new AABB();
	}
	aabb->SetNegativeInfinity();
	aabb->Enclose((float3*)tmp->vertex, tmp->num_vertex);
}

void GameObject::RenderBoundingBox()
{
	glBegin(GL_LINES);
	glLineWidth(5.0f);

	glColor4f(White.r, White.g, White.b, White.a);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(bounding_box.Edge(i).a.x, bounding_box.Edge(i).a.y, bounding_box.Edge(i).a.z);
		glVertex3f(bounding_box.Edge(i).b.x, bounding_box.Edge(i).b.y, bounding_box.Edge(i).b.z);
	}
	glEnd();
}

void GameObject::CleanUp()
{
	this->GetComponentMesh()->CleanUp();
	this->GetComponentTexture()->CleanUp();

	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); iterator++)
	{
		delete (*iterator);
	}

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->CleanUp();
	}

	components.clear();

}

void GameObject::TransformGlobal(GameObject* GO)
{
	ComponentTransform* transform = GO->GetComponentTransform();
	transform->TransformGlobalMat(GO->parent->GetComponentTransform()->GetGlobalTransform());

	for (std::vector<GameObject*>::iterator tmp = GO->children.begin(); tmp != GO->children.end(); ++tmp)
	{
		TransformGlobal(*tmp);
	}
	
}