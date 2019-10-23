#include "GameObject.h"
#include "Component_Mesh.h"
#

#include "mmgr/mmgr.h"

GameObject::GameObject(std::string name)
{
	this->name = name;
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

void GameObject::Update()
{

}

void GameObject::CleanUp()
{
	this->GetComponentMesh()->CleanUp();

	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); iterator++)
	{
		delete (*iterator);
	}

	components.clear();

}