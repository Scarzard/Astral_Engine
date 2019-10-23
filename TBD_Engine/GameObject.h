#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "Component_Mesh.h"
#include "Component_Transform.h"



class GameObject
{
public:
	GameObject(std::string name);
	virtual ~GameObject();

	void Update();
	void CleanUp();

	Component* CreateComponent(Component::ComponentType type);

	ComponentMesh* GetComponentMesh();
	ComponentTransform* GetComponentTransform();


public:

	bool active = true;

	std::string name;
	uint id = -1;

	std::vector<Component*> components;
};

#endif