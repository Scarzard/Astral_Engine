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
class Importer;

class MeshLoader : public Module
{
public:
	MeshLoader(bool start_enabled = true);
	~MeshLoader();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LoadFile(const char* path);
	void LoadNode(const aiScene * scene, aiNode * node, GameObject* parent, const char* full_path, Importer ex, std::string output_file);

};


#endif //__MESHLOADER_H__ 