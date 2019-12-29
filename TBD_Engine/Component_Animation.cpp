#include "Component_Animation.h"
#include "Application.h"
#include "ModuleInput.h"
#include "W_Game.h"
#include "GameObject.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

float Animation::GetDuration()
{
	return ((float)end - (float)start) / ticksXsecond;
}

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
		std::vector<GameObject*> childs;
		my_GO->GetAllChilds(childs);
		has_skeleton = HasSkeleton(childs);

		DoLink();
		playing_animation = CreateAnimation("Idle", 0, 48, true, true);
		CreateAnimation("Run", 50, 72, true);
		CreateAnimation("Punch", 73, 140, false);
	}

	if (linked_bones == false)
		DoBoneLink();
		
	//if (!App->gui->game_window->in_editor )
	//{
	//	if (!App->time->game_paused)
	//	{
	//		time += App->GetDT();

	//		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//		{
	//			//prev_anim = playing_animation;
	//			playing_animation = animations[2];
	//			time = 0;
	//		}

	//		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN ) // press key
	//		{
	//			//prev_anim = playing_animation;

	//			animations[1]->Default = true;
	//			animations[0]->Default = false;

	//			if (playing_animation->loop)
	//			{
	//				playing_animation = animations[1];
	//				time = 0;
	//			}
	//			
	//		}
	//		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP) //release key
	//		{
	//			animations[1]->Default = false;
	//			animations[0]->Default = true;

	//			if (playing_animation->loop)
	//			{
	//				playing_animation = GetDefaultAnimation();
	//				time = 0;
	//			}
	//			
	//		}
	//	}
	//	
	//}
	//else
	//{
	//	time = 0;
	//	playing_animation = GetDefaultAnimation();
	//}
	//Updating animation blend
	float extra_time = 0.0f;
	if (duration > 0.0f)
	{
		prev_time += dt;
		blending += dt;
	
		if (blending >= duration)
		{
			duration = 0.0f;
		}

		else if (prev_time >= animations[prev].GetDuration())
		{
			if (animations[prev].loop == true)
			{
				prev_time = 0.0f;
				// + (currentFrame - endFrame);
			}
		}

		if (duration > 0.0f)
			extra_time = blending / duration;
	}
	//Endof Updating animation blend

	time += dt;

	if (time > animations[current_index].GetDuration())
	{
		if (animations[current_index].loop == true)
		{
			time = 0.0f;
		}
	}
	if(extra_time > 0.0f)
		UpdateJointsTransform(animations[current_index], animations[prev], extra_time);
	else
		UpdateJointsTransform(animations[current_index], nullptr, extra_time);
	
	if (has_skeleton)
		UpdateMesh(my_GO);


	//Skinning
	
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

void ComponentAnimation::DoBoneLink()
{
	std::map<uint, ComponentMesh*> meshes;
	std::vector<ComponentBone*> bones;
	GetAllBones(my_GO, meshes, bones);

	for (uint i = 0; i < bones.size(); i++)
	{
		uint tmp_id = bones[i]->res_bone->meshID;

		//They have to have the same ID (Mesh/Bone), that's how they are linked
		std::map<uint, ComponentMesh*>::iterator it = meshes.find(tmp_id);
		if (it != meshes.end())
		{
			it->second->AddBone(bones[i]);
		}
	}

	linked_bones = true;
}

void ComponentAnimation::UpdateJointsTransform(const Animation* playing, const Animation* blend_to, float percentage)
{
	//// ----------------------- Frame count managment -----------------------------------
	int frame = 0;
	int prev_frame = 0;

	if (playing->ticksXsecond > 0.0f)
		frame = playing->start + res_anim->ticksPerSecond;
	else
		frame = (playing->start + res_anim->ticksPerSecond) * time;

	if (blend_to != nullptr)
	{
		if (blend_to->ticksXsecond > 0.0f)
			prev_frame = blend_to->start + res_anim->ticksPerSecond;
		else
			prev_frame = (blend_to->start + res_anim->ticksPerSecond) * time;
	}

	for (int i = 0; i < links.size(); i++)
	{
		ComponentTransform* trans = links[i].gameObject->GetComponentTransform();

		
		////
		//int Frame = playing_animation->start + (time * res_anim->ticksPerSecond);

		//if (Frame == playing_animation->end)
		//{

		//	if (!playing_animation->loop)
		//		if (playing_animation->Default == false)
		//			playing_animation = GetDefaultAnimation();
		//	
		//	time = 0;
		//	
		//}
		ChannelType* tmp = UpdateAllTRSChannel(links[i], frame, trans->GetPosition(), trans->GetQuaternionRotation(), trans->GetScale(), *playing);
		
		float3 p = tmp->channel_position;
		Quat r = tmp->channel_rotation;
		float3 s = tmp->channel_scale;

		if (blend_to != nullptr)
		{
			ChannelType* tmp1 = UpdateAllTRSChannel(links[i], frame, trans->GetPosition(), trans->GetQuaternionRotation(), trans->GetScale(), *blend_to);

			p = float3::Lerp(tmp1->channel_position, p, percentage);
			r = Quat::Slerp(tmp1->channel_rotation, r, percentage);
			s = float3::Lerp(tmp1->channel_scale, s, percentage);
		}
		
		trans->SetPosition(p);
		trans->SetQuatRotation(r);
		trans->SetScale(s);
		//-------------------------------------------------------------------------------------

		// POSITION
		//float3 position = trans->GetPosition();
		//if (links[i].channel->PosHasKey())
		//{
		//	
		//	std::map<double, float3>::iterator pos = links[i].channel->PositionKeys.find(Frame);
		//	if (pos != links[i].channel->PositionKeys.end())
		//		position = pos->second;
		//	else
		//	{
		//		Blend prev with next
		//		std::map<double, float3>::iterator prev = links[i].channel->PrevPosition(Frame);
		//		std::map<double, float3>::iterator next = links[i].channel->NextPosition(Frame);

		//		if (next == links[i].channel->PositionKeys.end())
		//			next = prev;
		//		else
		//		{
		//			float value = (Frame - prev->first) / (next->first - prev->first);
		//			position = prev->second.Lerp(next->second, value);
		//		}
		//	}

		//}
		//trans->SetPosition(position);

		//ROTATION
		//Quat rotation = trans->GetQuaternionRotation();
		//if (links[i].channel->RotHasKey())
		//{
		//	std::map<double, Quat>::iterator rot = links[i].channel->RotationKeys.find(Frame);
		//	if (rot != links[i].channel->RotationKeys.end())
		//		rotation = rot->second;
		//	else
		//	{
		//		Blend prev with next
		//		std::map<double, Quat>::iterator prev = links[i].channel->PrevRotation(Frame);
		//		std::map<double, Quat>::iterator next = links[i].channel->NextRotation(Frame);

		//		if (next == links[i].channel->RotationKeys.end())
		//			next = prev;
		//		else
		//		{
		//			float value = (Frame - prev->first) / (next->first - prev->first);
		//			rotation = prev->second.Lerp(next->second, value);
		//		}
		//	}
		//}
		//trans->SetQuatRotation(rotation);

		//SCALE
		//float3 scale = trans->GetScale();
		//if (links[i].channel->ScaleHasKey())
		//{
		//	std::map<double, float3>::iterator sca = links[i].channel->ScaleKeys.find(Frame);
		//	if (sca != links[i].channel->ScaleKeys.end())
		//		scale = sca->second;
		//	else
		//	{
		//		Blend prev with next
		//		std::map<double, float3>::iterator prev = links[i].channel->PrevScale(Frame);
		//		std::map<double, float3>::iterator next = links[i].channel->NextScale(Frame);

		//		if (next == links[i].channel->ScaleKeys.end())
		//			next = prev;
		//		else
		//		{
		//			float value = (Frame - prev->first) / (next->first - prev->first);
		//			scale = prev->second.Lerp(next->second, value);
		//		}
		//	}

		//}
		//trans->SetScale(scale);
		
	}
}

void ComponentAnimation::UpdateMesh(GameObject* go)
{
	ComponentMesh* tmp = go->GetComponentMesh();

	if (tmp != nullptr)
		tmp->UpdateDefMesh();

	for (int i = 0; i < go->children.size(); i++)
	{
		UpdateMesh(go->children[i]);
	}
}

void ComponentAnimation::GetAllBones(GameObject* go, std::map<uint, ComponentMesh*>& meshes, std::vector<ComponentBone*>& bones)
{
	ComponentMesh* mesh = go->GetComponentMesh();
	if (mesh != nullptr)
	{
		meshes[mesh->res_mesh->GetUUID()] = mesh;
	}

	ComponentBone* bone = go->GetComponentBone();
	if (bone != nullptr)
	{
		bones.push_back(bone);
	}

	for (uint i = 0; i < go->children.size(); i++)
	{
		GetAllBones(go->children[i], meshes, bones);
	}
}

ChannelType* ComponentAnimation::UpdateAllTRSChannel(Link & link, float current, float3 pos, Quat rot, float3 sc, const Animation & anim)
{
	//We need to return this variable to access channels in struct
	ChannelType* type = nullptr;

	float3 position = pos;

	if (link.channel->PosHasKey())
	{
		std::map<double, float3>::iterator prev = link.channel->PrevPosition(current, anim.start, anim.end);
		std::map<double, float3>::iterator next = link.channel->NextPosition(current, anim.start, anim.end);

		if (next == link.channel->PositionKeys.end())
			next = prev;

		if (prev == next)
			position = prev->second;
		else 
		{
			float value = (current - prev->first) / (next->first - prev->first);
			position = prev->second.Lerp(next->second, value);
		}
	}

	type->channel_position = position;

	Quat rotation = rot;

	if (link.channel->RotHasKey())
	{
		std::map<double, Quat>::iterator prev = link.channel->PrevRotation(current, anim.start, anim.end);
		std::map<double, Quat>::iterator next = link.channel->NextRotation(current, anim.start, anim.end);

		if (next == link.channel->RotationKeys.end())
			next = prev;

		if (prev == next)
			rotation = prev->second;
		else
		{
			float value = (current - prev->first) / (next->first - prev->first);
			rotation = prev->second.Slerp(next->second, value);
		}
	}

	type->channel_rotation = rotation;

	float3 scale = sc;

	if (link.channel->ScaleHasKey())
	{
		std::map<double, float3>::iterator prev = link.channel->PrevScale(current, anim.start, anim.end);
		std::map<double, float3>::iterator next = link.channel->NextScale(current, anim.start, anim.end);

		if (next == link.channel->ScaleKeys.end())
			next = prev;

		if (prev == next)
			scale = prev->second;
		else 
		{
			float value = (current - prev->first) / (next->first - prev->first);
			scale = prev->second.Lerp(next->second, value);
		}
	}

	type->channel_scale = scale;

	return type;
}

bool ComponentAnimation::HasSkeleton(std::vector<GameObject*> collector)
{
	for (int i = 0; i < collector.size(); i++)
		if (collector[i]->GetComponentBone())return true;

	return false;
}


