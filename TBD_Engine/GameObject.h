#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "Component_Mesh.h"
#include "Component_Transform.h"
#include "Component_Texture.h"
#include "Component_Camera.h"
#include "Component_Animation.h"
#include "Component_Bone.h"

class GameObject
{
public:
	GameObject(std::string name);
	virtual ~GameObject();

	void Update(float dt);
	void CleanUp();

	// Load & Save 
	void Load(uint obj_num, nlohmann::json &scene_file);
	void Save(uint obj_num, nlohmann::json &scene_file);

	void Enable();
	void Disable();

	Component* CreateComponent(Component::ComponentType type);

	ComponentMesh* GetComponentMesh();
	ComponentTransform* GetComponentTransform();
	ComponentTexture* GetComponentTexture();
	ComponentCamera* GetComponentCamera();
	ComponentAnimation* GetComponentAnimation();
	ComponentBone* GetComponentBone();

	void TransformGlobal(GameObject* GO);

	void DeleteGO(GameObject* GO, bool original);
	void SetChild(GameObject* GO);
	void RemoveChild(GameObject* GO);
	void GetAllChilds(std::vector<GameObject*>& vector);
	GameObject * GetAnimGO(GameObject* go);

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
};

#endif