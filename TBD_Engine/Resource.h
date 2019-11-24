#ifndef __Resource_H__
#define __Resource_H__

#include "Globals.h"



class Resource
{

public:
	enum RES_TYPE {
		UNKNOWN = 1,
		TEXTURE,
		MESH,
		MODEL,
		SCENE		
	};

	Resource(uint UUID, RES_TYPE type) : res_UUID(UUID), type(type) {}
	virtual ~Resource() {}

	// Getters ----
	uint GetUUID() const { return res_UUID; }
	const char* GetFile() const { return file.c_str(); }
	const char* GetExportedFile() const { return exported_file.c_str(); }
	RES_TYPE GetType() const { return type; };
	uint GetNumReferences() const { return loaded; }
	
	void UpdateNumReference()
	{
		if (loaded > 0)
			loaded++;
		else
		{
			if (LoadInMemory())
				loaded = 1;
			else
				loaded = 0;
		}
	}

	//virtual void Load(const nlohmann::json &config);
	//virtual void Save(nlohmann::json &config) const;
	virtual bool LoadInMemory() = 0;
	virtual void ReleaseMemory() = 0;

public:
	uint res_UUID = 0;
	std::string file = "";
	std::string exported_file = "";
	RES_TYPE type = RES_TYPE::UNKNOWN;

	uint loaded = 0;
};

#endif // __Resource_H__