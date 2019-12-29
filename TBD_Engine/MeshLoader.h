#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include "Globals.h"
#include "Module.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "Assimp/include/cimport.h"

class GameObject;
class aiMesh;
class aiNode;
class aiScene;
class aiBone;
class aiNodeAnim;
class Importer;
class ResourceMesh;
class ResourceBone;
class ResourceAnimation;
class Channel;

class MeshLoader : public Module
{
public:
	ALIGN_CLASS_TO_16

	MeshLoader(bool start_enabled = true);
	~MeshLoader();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LoadFile(const char* path);
	void LoadNode(const aiScene* scene, aiNode* node, GameObject* parent, const char* full_path, std::string output_file, std::vector<aiMesh*>& mesh_collect, std::vector<GameObject*>& go_collect);
	void LoadBones(std::vector<aiMesh*> mesh_collect, std::vector<GameObject*> go_collect, GameObject* root, const char* full_path);
	void LoadChannel(const aiNodeAnim* AnimNode, Channel& channel);

	//Export .mesh
	bool Export(const char * name, std::string & output_file, uint num_index, uint* index, uint num_vertex, float3* vertex,uint num_normals, float3* face_center, float3* face_normal, uint num_tex_coords, float* tex_coords);
	//Import .mesh
	bool Load(ResourceMesh* mesh);

	//Export .anim
	bool ExportAnim(std::string & output_file, std::string name, float duration, float ticksPerSecond, uint numChannels, aiNodeAnim** mChannels);
	//Import .anim
	bool LoadAnim(ResourceAnimation* anim);

	//Export .bone
	bool ExportBone(std::string & output_file, ResourceBone* tmp, aiBone* mBones, uint id);

	void FillMap(std::map<std::string, GameObject*>& map, GameObject* root);
	void LoadBoneData(const aiBone* bone, ResourceBone* res_bone, uint mesh_id);

};


#endif //!__MESHLOADER_H__ 