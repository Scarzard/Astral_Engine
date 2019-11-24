#ifndef __ResourceTexture_H__
#define __ResourceTexture_H__

#include "Resource.h"
class ResourceTexture : public Resource
{
public:
	ResourceTexture(uint uuid) : Resource(uuid, RES_TYPE::TEXTURE) {}

	virtual ~ResourceTexture() {}

	//void Save(const char* resourceNum, nlohmann::json &config) const;
	//void Load(const char* resourceNum, const nlohmann::json &config);

	bool LoadInMemory();
	void ReleaseMemory();

public:
	uint texture = 0;
	uint width = 0;
	uint height = 0;
};

#endif // __ResourceTexture_H__