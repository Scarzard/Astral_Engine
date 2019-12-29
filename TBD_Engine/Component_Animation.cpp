#include "Component_Animation.h"
#include "Application.h"
#include "ModuleInput.h"
#include "W_Game.h"
#include "GameObject.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "mmgr/mmgr.h"

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

	if (end_position != nullptr)
	{
		delete[] end_position;
		delete[] end_rotation;
		delete[] end_scale;
		delete[] start_position;
		delete[] start_rotation;
		delete[] start_scale;
	}
	

	if (res_anim)
	{
		res_anim->loaded -= 1;
		if (res_anim->loaded == 0)
			res_anim->ReleaseMemory();
	}
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
		

	if (!App->gui->game_window->in_editor )
	{
		if (!App->time->game_paused)
		{
			


			if (blending == false)
			{
				time += App->GetDT();
				UpdateJointsTransform();
			}	
			else
				BlendAnimations();

			if (has_skeleton)
				UpdateMesh(my_GO);

			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				//prev_anim = playing_animation;
				StartBlend(animations[2]->start);
				playing_animation = animations[2];
				time = 0;
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN ) // press key
			{
				//prev_anim = playing_animation;

				animations[1]->Default = true;
				animations[0]->Default = false;

				if (playing_animation->loop)
				{
					StartBlend(animations[1]->start);
					playing_animation = animations[1];
					time = 0;
				}
				
			}
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP) //release key
			{
				animations[1]->Default = false;
				animations[0]->Default = true;

				if (playing_animation->loop)
				{
					StartBlend(GetDefaultAnimation()->start);
					playing_animation = GetDefaultAnimation();
					time = 0;
				}
				
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

void ComponentAnimation::UpdateJointsTransform()
{
	for (int i = 0; i < links.size(); i++)
	{
		ComponentTransform* trans = links[i].gameObject->GetComponentTransform();

		// ----------------------- Frame count managment -----------------------------------
		
		Frame = playing_animation->start + (time * res_anim->ticksPerSecond);

		if (Frame == playing_animation->end)
		{

			if (!playing_animation->loop)
				if (playing_animation->Default == false)
				{
					StartBlend(GetDefaultAnimation()->start);
					playing_animation = GetDefaultAnimation();
				}
					
			
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
					rotation = prev->second.Slerp(next->second, value);
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

void ComponentAnimation::StartBlend(uint start)
{
	blend_start_Frame = start;

	if (end_position == nullptr)
	{
		end_position = new float3[links.size()];
		end_rotation = new Quat[links.size()];
		end_scale = new float3[links.size()];

		start_position = new float3[links.size()];
		start_rotation = new Quat[links.size()];
		start_scale = new float3[links.size()];
	}

	for (int i = 0; i < links.size(); i++)
	{
		ComponentTransform* trans = links[i].gameObject->GetComponentTransform();

		end_position[i] = trans->GetPosition();
		end_rotation[i] = trans->GetQuaternionRotation();
		end_scale[i] = trans->GetScale();

		std::map<double, float3>::iterator next_pos = links[i].channel->PositionKeys.find(blend_start_Frame);
		if (next_pos != links[i].channel->PositionKeys.end())
			start_position[i] = next_pos->second;
		else
			start_position[i] = float3(1234, 0, 0);

		std::map<double, Quat>::iterator next_rot = links[i].channel->NextRotation(blend_start_Frame);
		if (next_rot != links[i].channel->RotationKeys.end())
			start_rotation[i] = next_rot->second;
		else
			start_rotation[i] = Quat(1234, 0,0,0);

		std::map<double, float3>::iterator next_sc = links[i].channel->NextScale(blend_start_Frame);
		if (next_sc != links[i].channel->ScaleKeys.end())
			start_scale[i] = next_sc->second;
		else
			start_scale[i] = float3(1234,0,0);
	}

	blending = true;
}

void ComponentAnimation::BlendAnimations(float blend_time)
{
	curr_blend_time += App->GetDT();
	
	float value = curr_blend_time / blend_time;

	for (int i = 0; i < links.size(); i++)
	{
		ComponentTransform* trans = links[i].gameObject->GetComponentTransform();

		if(start_position[i].x != 1234)
			trans->SetPosition(end_position[i].Lerp(start_position[i], value));

		if (start_rotation[i].x != 1234 )
			trans->SetQuatRotation(end_rotation[i].Slerp(start_rotation[i], value));

		if (start_scale[i].x != 1234)
			trans->SetScale(end_scale[i].Lerp(start_scale[i], value));

	}

	App->LogInConsole("%f", curr_blend_time);

	if (curr_blend_time >= blend_time)
	{
		blending = false;
		curr_blend_time = 0;
	}
		
}

void ComponentAnimation::UpdateMesh(GameObject* go)
{
	ComponentMesh* tmp = go->GetComponentMesh();

	if (tmp != nullptr)
	{
		//tmp->AttachSkeleton(go);
		tmp->UpdateDefMesh();

		if (tmp->deformable_mesh != nullptr)
		{
			//Vertex buffer
			if (!created_buffer)
			{
				
				created_buffer = true;
			}
			
			
		}

	}

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

bool ComponentAnimation::HasSkeleton(std::vector<GameObject*> collector)
{
	for (int i = 0; i < collector.size(); i++)
		if (collector[i]->GetComponentBone())return true;

	return false;
}


