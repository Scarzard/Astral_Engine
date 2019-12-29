#ifndef __ResourceBone_H__
#define __ResourceBone_H__

#include "Resource.h"
#include "MathGeoLib/include/MathGeoLib.h"


class ResourceBone : public Resource
{
public:
	ALIGN_CLASS_TO_16

	ResourceBone(uint uuid) : Resource(uuid, RES_TYPE::BONE) {}
	virtual ~ResourceBone() {}

	bool LoadInMemory();
	void ReleaseMemory();

	uint meshID = 0;

	uint NumWeights = 0;
	float* weight = nullptr; //weight value
	uint* index_weight = nullptr;
	
	float4x4 matrix = float4x4::identity;	
};

#endif // __ResourceBone_H__