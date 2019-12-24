#include "Component_Animation.h"

float AnimParameters::GetDuration()
{
	return ((float)end - (float)start) / ticksPerSecond;
}

ComponentAnimation::ComponentAnimation(GameObject* gameobj) : Component(Component::ComponentType::Animation, gameobj)
{

}

ComponentAnimation::~ComponentAnimation()
{

}

void ComponentAnimation::Update(float dt)
{
}
