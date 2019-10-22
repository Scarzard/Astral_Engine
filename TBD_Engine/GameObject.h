#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "Component_Mesh.h"


class GameObject
{
public:
	GameObject(std::string name);
	virtual ~GameObject();

	void Update();

	Component* CreateComponent(Component::ComponentType type);

	ComponentMesh* GetComponentMesh();


public:

	bool active = true;
	std::string name;

	std::vector<Component*> components;
};

#endif