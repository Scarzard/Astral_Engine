#include "GameObject.h"
#include "Component_Mesh.h"
#

#include "mmgr/mmgr.h"

GameObject::GameObject(char* name)
{
	this->name = name;
}

GameObject::~GameObject(){}

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

void GameObject::Update()
{

}