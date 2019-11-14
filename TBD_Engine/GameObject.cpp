#include "GameObject.h"

#include "mmgr/mmgr.h"

GameObject::GameObject(std::string name)
{
	this->name = name;
	unactive_name = name + " (not active)";
	this->active = true;
	CreateComponent(Component::ComponentType::Transform);
	CreateComponent(Component::ComponentType::Mesh);
	CreateComponent(Component::ComponentType::Texture);
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
	Component* transform = nullptr;
	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); iterator++)
	{
		if ((*iterator)->type == Component::ComponentType::Transform)
		{
			return (ComponentTexture*)*iterator;
		}
	}

	return (ComponentTexture*)transform;
}

void GameObject::Update(float dt)
{
	//update component transform

	//update name when GO change state
	std::string str = this->name + " (not active)";
	if(unactive_name.compare(str) != 0)
		unactive_name = name + " (not active)";

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->Update(dt);
	}
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
	/*ComponentTransform* transform = GO->GetComponentTransform();
	transform->TransformGlobalMat(transform->GetGlobalTransform());*/
}