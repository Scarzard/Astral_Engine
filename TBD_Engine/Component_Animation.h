#ifndef __COMPONENT_ANIM_H__
#define __COMPONENT_ANIM_H__

#include "Component.h"
#include <vector>
#include <map>
#include "ResourceAnimation.h"

struct Animation
{
	Animation(std::string name, uint start, uint end, bool loop) : name(name), start(start), end(end), loop(loop) {};

	std::string name;
	uint start;
	uint end;
	bool loop = false;
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
	float GetDuration() { return res_anim->duration / res_anim->ticksPerSecond; }

public:

	std::vector<Animation*> animations;
	Animation* playing_animation;

	ResourceAnimation* res_anim = nullptr;

private:

	void CreateAnimation(std::string name, uint start, uint end, bool loop);
	void DoLink();
	void UpdateJointsTransform(float dt);

	std::vector<Link> links;
	bool linked_channels = false;

	uint loop_times = 0;
	float time = 0;
};

#endif // __COMPONENT_ANIM_H__