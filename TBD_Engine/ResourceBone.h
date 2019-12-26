#ifndef __ResourceBone_H__
#define __ResourceBone_H__

#include "Resource.h"
#include "MathGeoLib/include/MathGeoLib.h"


class ResourceBone : public Resource
{
public:
	ResourceBone(uint uuid) : Resource(uuid, RES_TYPE::BONE) {}
	~ResourceBone() {}

	bool LoadInMemory() { return true; }
	void ReleaseMemory() {}

	uint meshID = 0;

	uint NumWeights = 0;
	float* weight = nullptr; //weight value
	uint* index_weight = nullptr; //vertex id
	
	float4x4 matrix = float4x4::identity;	
};

#endif // __ResourceBone_H__