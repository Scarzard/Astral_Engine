#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "Component_Mesh.h"
#include "Component_Transform.h"
#include "Component_Texture.h"



class GameObject
{
public:
	GameObject(std::string name);
	virtual ~GameObject();

	void Update(float dt);
	void CleanUp();

	void Enable();
	void Disable();

	Component* CreateComponent(Component::ComponentType type);

	ComponentMesh* GetComponentMesh();
	ComponentTransform* GetComponentTransform();
	ComponentTexture* GetComponentTexture();

	void TransformGlobal(GameObject* GO);

	void DeleteGO(GameObject* GO);
	void SetChild(GameObject* GO);
	void RemoveChild(GameObject* GO);


public:

	bool active = true;

	std::string name;
	std::string unactive_name;

	uint id = -1;

	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
};

#endif