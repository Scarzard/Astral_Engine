#ifndef __ModuleResources_H__
#define __ModuleResources_H__

#include "Module.h"
#include "Resource.h"

class ResourceTexture;

class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true) : Module(start_enabled) {}

	virtual ~ModuleResources() {}
	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void DrawExplorer();

	Resource* NewResource(Resource::RES_TYPE type);
	uint GetNewFile(const char* new_file);
	uint ImportFile(const char* new_file_in_assets, Resource::RES_TYPE type);

	// Getters ------------
	Resource* Get(uint uid);
	uint GetResourceInAssets(const char* file)const;
	uint IsResourceInLibrary(const char* name) const;

public:
	std::map<uint, Resource*> resources;

	//-------- For the assets window --------------
	std::map<uint, ResourceTexture*> tex_resources;
	//---------------------------------------------

private:
	ResourceTexture* mesh_icon = nullptr;
};

#endif //!__ModuleResources_H__