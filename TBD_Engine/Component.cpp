#include "Component.h"

#include "mmgr/mmgr.h"

Component::Component(Component::ComponentType type, GameObject* GO)
{
	this->type = type;
	my_GO = GO;
}


Component::~Component()
{
}

void Component::Update()
{
}

void Component::Enable()
{
	if(!active)
		active = true;
}

void Component::Disable()
{
	if (active)
		active = false;
}