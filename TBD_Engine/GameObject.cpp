#include "GameObject.h"

#include "mmgr/mmgr.h"

GameObject::GameObject(std::string name)
{
	this->name = name;
	CreateComponent(Component::ComponentType::Transform);
	CreateComponent(Component::ComponentType::Mesh);
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

void GameObject::Update()
{

}

void GameObject::CleanUp()
{
	this->GetComponentMesh()->CleanUp();
	this->GetComponentTransform()->CleanUp();

	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); iterator++)
	{
		delete (*iterator);
	}

	components.clear();

}