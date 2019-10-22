#ifndef __GAMEOOBJECT_H__
#define __GAMEOOBJECT_H__

#include "Globals.h"
#include "Component.h"

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