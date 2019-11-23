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
		Texture
	};

	Component(Component::ComponentType type, GameObject* GO);
	virtual ~Component();

	virtual void Enable();
	virtual bool Update();
	virtual void Disable();

	// Load & Save 
	virtual void Load(uint obj_num, const nlohmann::json &scene_file) {}
	virtual void Save(uint obj_num, nlohmann::json &scene_file) {}


public:

	Component::ComponentType type;
	bool active = true;
	GameObject* my_GO = nullptr;
	uint UUID = 0;
};

#endif