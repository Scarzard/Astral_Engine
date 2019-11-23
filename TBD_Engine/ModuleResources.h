#ifndef __ModuleResources_H__
#define __ModuleResources_H__

#include "Module.h"
#include "Resource.h"
#include "Importer.h"


class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true) : Module(start_enabled) {}

	virtual ~ModuleResources() {}
	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();


	Resource* NewResource(Resource::RES_TYPE type);
	uint GetNewFile(const char* new_file);
	uint ImportFile(const char* new_file_in_assets, Resource::RES_TYPE type);

	uint Find(const char* file_in_assets) const;

	//uint GenerateNewUID() {  }

	// Getters ------------
	Resource* Get(uint uid);

private:
	uint ResourceInAssets(const char* file)const;
	void DrawExplorer();

private:
	std::map<uint, Resource*> resources;
	Importer importer;
};

#endif //!__ModuleResources_H__