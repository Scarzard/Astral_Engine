#ifndef __ANIM_LOADER_H__
#define __ANIM_LOADER_H__

#include "Module.h"
#include "ResourceAnimation.h"
#include "GameObject.h"

#include <string>
#include <map>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct aiScene;
struct aiAnimation;
struct aiNodeAnim;
struct aiMesh;
struct aiBone;

class AnimationLoader : public Module
{
public:
	AnimationLoader(bool start_enabled = true);
	~AnimationLoader();

	//Animation data management
	uint ImportGOAnimations(const aiScene* scene, GameObject* root, const char* source_file);
	ResourceAnimation* ImportAnimation(const aiAnimation* tmp_animation, uint ID, const char* source_file);


private:
	void CollectGameObjectNames(GameObject* gameObject, std::map<std::string, GameObject*>& map);
	void LoadChannel(const aiNodeAnim* node, Channel& channel);
};

#endif //!__ANIM_LOADER_H__