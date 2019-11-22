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

	void DeleteGO(GameObject* GO, bool original);
	void SetChild(GameObject* GO);
	void RemoveChild(GameObject* GO);

public:

	bool active = true;
	bool is_static = true;

	//id just for naming
	uint id = -1;

	//random id
	uint UUID = 0;

	//name
	std::string name;

	//display this name when unactive
	std::string unactive_name;

	GameObject* parent = nullptr;
	std::vector<Component*> components;
	std::vector<GameObject*> children;


private:
	void RenderBoundingBox();
};

#endif