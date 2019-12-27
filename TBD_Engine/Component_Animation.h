#ifndef __COMPONENT_ANIM_H__
#define __COMPONENT_ANIM_H__

#include "Component.h"
#include <vector>
#include <map>
#include "ResourceAnimation.h"

struct AnimParameters
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

	std::vector<AnimParameters> animations;
	

	bool playing = false;

	ResourceAnimation* res_anim = nullptr;

private:

	void DoLink();
	void UpdateJointsTransform();

	std::vector<Link> links;
	bool linked_channels = false;
};

#endif // __COMPONENT_ANIM_H__