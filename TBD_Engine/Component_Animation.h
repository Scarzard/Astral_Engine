#ifndef __COMPONENT_ANIM_H__
#define __COMPONENT_ANIM_H__

#include "Component.h"
#include <vector>
#include <map>
#include "ResourceAnimation.h"

class ComponentMesh;
class ComponentBone;

struct Animation
{
	std::string name;

	uint start;
	uint end;

	float duration;
	float ticksPerSecond;

	bool loop = false;
	bool idle = true;

	float GetDuration();
};

class ComponentAnimation : public Component
{
	struct Link //Stores a bone(gameobject) with his corresponding channel of the animation
	{
		Link(GameObject* gameObject, Channel* channel) : gameObject(gameObject), channel(channel) {};
		GameObject* gameObject;
		Channel* channel;
	};

public:
	ComponentAnimation(GameObject* gameObject);
	~ComponentAnimation();

	void Update(float dt);

public:

	std::vector<Animation> animations;
	

	bool playing = false;

	ResourceAnimation* res_anim = nullptr;

private:

	void DoLink();
	void DoBoneLink();
	void UpdateJointsTransform(float dt);
	void UpdateMesh(GameObject* go);

	void GetAllBones(GameObject* go, std::map<uint, ComponentMesh*>& meshes, std::vector<ComponentBone*>& bones);

	std::vector<Link> links;
	bool linked_channels = false;
	bool linked_bones = false;

	uint loop_times = 0;
	float time = 0;
};

#endif // __COMPONENT_ANIM_H__