#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"

class GameObject;

class Component
{
public:
	enum class ComponentType
	{
		NONE = -1,
		Transform,
		Mesh,
		Texture
	};

	Component(Component::ComponentType type, GameObject* GO);
	virtual ~Component();

	virtual void Enable();
	virtual bool Update();
	virtual void Disable();

public:

	Component::ComponentType type;
	bool active = true;
	GameObject* my_GO = nullptr;
};

#endif