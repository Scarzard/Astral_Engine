#include "Component_Animation.h"
#include "Application.h"
#include "W_Game.h"
#include "GameObject.h"

ComponentAnimation::ComponentAnimation(GameObject* gameobj) : Component(Component::ComponentType::Animation, gameobj)
{

}

ComponentAnimation::~ComponentAnimation()
{
	for (int i = 0; i < animations.size(); i++)
	{
		delete animations[i];
	}

	animations.clear();
}

void ComponentAnimation::CreateAnimation(std::string name, uint start, uint end, bool loop)
{
	Animation* anim = new Animation(name, start, end, loop);
	animations.push_back(anim);
}

void ComponentAnimation::Update(float dt)
{
	if (linked_channels == false)
	{
		DoLink();
		CreateAnimation("Idle", 0, 49, true);
		CreateAnimation("Run", 50, 72, true);
		CreateAnimation("Punch", 73, 138, false);
	}
		

	if (!App->gui->game_window->in_editor)
		UpdateJointsTransform(dt);
	else
		time = 0;
}

void ComponentAnimation::DoLink()
{
	if (res_anim != nullptr)
	{
		std::vector<GameObject*> childs;
		my_GO->GetAllChilds(childs);

		for (int i = 0; i < res_anim->numChannels; i++)
		{
			for (int j = 0; j < childs.size(); j++)
			{
				// We link them if the GO is a bone and their names are equal
				if (childs[j]->GetComponentBone() != nullptr && childs[j]->name.compare(res_anim->channels[i].name) == 0) 
				{
					links.push_back(Link(childs[j], &res_anim->channels[i]));
					break;
				}
			}
		}

		linked_channels = true;
	}
	else
	{
		App->LogInConsole("ERROR: Can't link channels, Resource_Animation is nullptr");
	}
}

void ComponentAnimation::UpdateJointsTransform(float dt)
{
	for (int i = 0; i < links.size(); i++)
	{
		ComponentTransform* trans = links[i].gameObject->GetComponentTransform();
		float duration_sec = GetDuration();

		// ----------------------- Frame count managment -----------------------------------
		if (App->time->GetGameTime() > duration_sec)
		{
			time = App->time->GetGameTime() - duration_sec * loop_times;
		}
		else
		{
			time = App->time->GetGameTime();
		}
		uint Frame = time * res_anim->ticksPerSecond;

		if (Frame == res_anim->duration)
		{
			loop_times++;
		}
		//-------------------------------------------------------------------------------------

		// POSITION
		float3 position = trans->GetPosition();
		if (links[i].channel->PosHasKey())
		{
			
			std::map<double, float3>::iterator pos = links[i].channel->PositionKeys.find(Frame);
			if (pos != links[i].channel->PositionKeys.end())
				position = pos->second;
			else
			{
				//Blend prev with next
				std::map<double, float3>::iterator prev = links[i].channel->PrevPosition(Frame);
				std::map<double, float3>::iterator next = links[i].channel->NextPosition(Frame);

				if (next == links[i].channel->PositionKeys.end())
					next = prev;
				else
				{
					float value = (Frame - prev->first) / (next->first - prev->first);
					position = prev->second.Lerp(next->second, value);
				}
			}

		}
		trans->SetPosition(position);

		//ROTATION
		Quat rotation = trans->GetQuaternionRotation();
		if (links[i].channel->RotHasKey())
		{
			std::map<double, Quat>::iterator rot = links[i].channel->RotationKeys.find(Frame);
			if (rot != links[i].channel->RotationKeys.end())
				rotation = rot->second;
			else
			{
				//Blend prev with next
				std::map<double, Quat>::iterator prev = links[i].channel->PrevRotation(Frame);
				std::map<double, Quat>::iterator next = links[i].channel->NextRotation(Frame);

				if (next == links[i].channel->RotationKeys.end())
					next = prev;
				else
				{
					float value = (Frame - prev->first) / (next->first - prev->first);
					rotation = prev->second.Lerp(next->second, value);
				}
			}
		}
		trans->SetQuatRotation(rotation);

		//SCALE
		float3 scale = trans->GetScale();
		if (links[i].channel->ScaleHasKey())
		{
			std::map<double, float3>::iterator sca = links[i].channel->ScaleKeys.find(Frame);
			if (sca != links[i].channel->ScaleKeys.end())
				scale = sca->second;
			else
			{
				//Blend prev with next
				std::map<double, float3>::iterator prev = links[i].channel->PrevScale(Frame);
				std::map<double, float3>::iterator next = links[i].channel->NextScale(Frame);

				if (next == links[i].channel->ScaleKeys.end())
					next = prev;
				else
				{
					float value = (Frame - prev->first) / (next->first - prev->first);
					scale = prev->second.Lerp(next->second, value);
				}
			}

		}
		trans->SetScale(scale);
		
		

	}
}
