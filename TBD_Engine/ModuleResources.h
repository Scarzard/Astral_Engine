#ifndef __ModuleResources_H__
#define __ModuleResources_H__

#include "Module.h"
#include "Resource.h"

class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true) : Module(start_enabled) {}

	virtual ~ModuleResources() {}
	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();


	Resource* NewResource(Resource::RESOURCE_TYPE type);
	uint ImportFile(const char* new_file_in_assets, Resource::RESOURCE_TYPE type, bool force = false);

	uint Find(const char* file_in_assets) const;

	uint GenerateNewUID() { return App->GetRandomUUID(); }

	// Getters ------------
	const Resource* Get(uint uid) const;
	Resource* Get(uint uid);


	void DrawExplorer();

private:
	std::map<uint, Resource*> resources;
};

#endif //!__ModuleResources_H__