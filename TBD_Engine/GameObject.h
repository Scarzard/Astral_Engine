#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Component.h"

class Component;

class GameObject
{
public:
	GameObject(char* name);
	virtual ~GameObject();

	void Update();

	Component* CreateComponent(Component::ComponentType type);

public:

	bool active = true;
	std::string name;

	std::vector<Component*> components;
};

#endif