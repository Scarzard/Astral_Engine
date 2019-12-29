#ifndef __ResourceAnimation_H__
#define __ResourceAnimation_H__

#include "Resource.h"
#include "MathGeoLib/include/MathGeoLib.h"

struct Channel 
{
	std::string name;

	//Position
	bool PosHasKey() const;
	std::map<double, float3> PositionKeys;
	std::map<double, float3>::iterator PrevPosition(double current, uint, uint);
	std::map<double, float3>::iterator NextPosition(double current, uint, uint);

	//Rotation
	bool RotHasKey() const;
	std::map<double, Quat> RotationKeys;
	std::map<double, Quat>::iterator PrevRotation(double current, uint, uint);
	std::map<double, Quat>::iterator NextRotation(double current, uint, uint);
															
	//Scale													
	bool ScaleHasKey() const;								
	std::map<double, float3> ScaleKeys;						
	std::map<double, float3>::iterator PrevScale(double current, uint, uint);
	std::map<double, float3>::iterator NextScale(double current, uint, uint);
};

class ResourceAnimation : public Resource
{
public:

	ResourceAnimation(uint uuid) : Resource(uuid, RES_TYPE::ANIMATION) {}

	virtual ~ResourceAnimation() {}

	bool LoadInMemory() { return true; }
	void ReleaseMemory() {}
	
	std::string name;

	float duration = 0.0f;
	float ticksPerSecond = 0.0f;
	bool loop = true;

	uint numChannels = 0;
	Channel* channels = nullptr;
};

#endif // __ResourceAnimation_H__