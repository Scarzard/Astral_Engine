#include "AnimationLoader.h"

#include "Application.h"

#include "ModuleResources.h"
#include "ResourceBone.h"
#include "ResourceAnimation.h"

#include "Component_Mesh.h"

#include "Assimp/include/scene.h"

AnimationLoader::AnimationLoader(bool start_enabled)
{
}

AnimationLoader::~AnimationLoader()
{
}

uint AnimationLoader::ImportGOAnimations(const aiScene * scene, GameObject * root, const char * source_file)
{
	uint ret = 0;
	if (scene->HasAnimations())
	{
		std::map<std::string, GameObject*> map;
		CollectGameObjectNames(root, map);

		for (uint i = 0; i < scene->mNumAnimations; i++)
		{
			//Import animation from function in App->resources
		}
	}
	return ret;
}

ResourceAnimation * AnimationLoader::ImportAnimation(const aiAnimation * tmp_animation, uint ID, const char * source_file)
{
	ResourceAnimation* animation = new ResourceAnimation(); //small error

	animation->duration = tmp_animation->mDuration;
	animation->ticksPerSecond = tmp_animation->mTicksPerSecond;
	animation->numChannels = tmp_animation->mNumChannels;
	animation->channels = new Channel[tmp_animation->mNumChannels];

	for (uint i = 0; i < tmp_animation->mNumChannels; i++)
		LoadChannel(tmp_animation->mChannels[i], animation->channels[i]);

	animation->res_UUID = ID;
	animation->name = tmp_animation->mName.C_Str();
	
	
	animation->LoadInMemory();

	return animation;
}
