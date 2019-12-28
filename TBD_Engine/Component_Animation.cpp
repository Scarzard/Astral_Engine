#include "Component_Animation.h"
#include "Application.h"
#include "ModuleInput.h"
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

Animation* ComponentAnimation::CreateAnimation(std::string name, uint start, uint end, bool loop, bool Default)
{
	Animation* anim = new Animation(name, start, end, loop, Default);
	animations.push_back(anim);

	return anim;
}

Animation* ComponentAnimation::GetDefaultAnimation()
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animations[i]->Default)
		{
			return animations[i];
		}
			
	}

	//normally first one is idle ?
	return animations[0];
}

void ComponentAnimation::Update(float dt)
{
	if (linked_channels == false)
	{
		DoLink();
		playing_animation = CreateAnimation("Idle", 0, 49, true, true);
		CreateAnimation("Run", 50, 72, true);
		CreateAnimation("Punch", 73, 140, false);
	}
		

	if (!App->gui->game_window->in_editor )
	{
		if (!App->time->game_paused)
		{
			time += App->GetDT();
			UpdateJointsTransform();

			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				prev_anim = playing_animation;
				playing_animation = animations[2];
				time = 0;
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN ) // press key
			{
				prev_anim = playing_animation;
				playing_animation = animations[1];
				time = 0;
			}
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP) //release key
			{
				playing_animation = GetDefaultAnimation();
				time = 0;
			}

		}
		
	}
	else
	{
		time = 0;
		playing_animation = GetDefaultAnimation();
	}
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

void ComponentAnimation::UpdateJointsTransform()
{
	for (int i = 0; i < links.size(); i++)
	{
		ComponentTransform* trans = links[i].gameObject->GetComponentTransform();

		// ----------------------- Frame count managment -----------------------------------
		
		int Frame = playing_animation->start + (time * res_anim->ticksPerSecond);

		if (Frame == playing_animation->end)
		{

			if (!playing_animation->loop)
				if (playing_animation->Default == false)
					playing_animation = GetDefaultAnimation();
			
			time = 0;
			
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
