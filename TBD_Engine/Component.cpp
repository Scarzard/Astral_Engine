#include "Component.h"
#include "Application.h"

#include "mmgr/mmgr.h"

Component::Component(Component::ComponentType type, GameObject* GO)
{
	this->type = type;
	my_GO = GO;
	if(type != ComponentType::Camera)
		UUID = App->GetRandomUUID();
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