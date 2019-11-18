#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"

#include "ImGui/imgui.h"

class GameObject;

class Component
{
public:
	enum class ComponentType
	{
		NONE = -1,
		Transform,
		Mesh,
		Texture,
		Camera
	};

	Component(Component::ComponentType type, GameObject* GO);
	virtual ~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

public:

	Component::ComponentType type;
	bool active = true;
	GameObject* my_GO = nullptr;
};

#endif