#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "Module.h"
#include "Globals.h"


class TimeManager : public Module
{
public:
	TimeManager(bool start_enabled = true);
	~TimeManager();

	update_status PostUpdate(float dt);
	bool CleanUp();

	void ResetGameTimer();

	uint GetFrameCount() const;
	float GetRealTimeClock() const;
	float GetRealDT() const;
	float GetGameTime() const;
	float GetTimeScale() const;
	float GetGameDT() const;

public:

	float time_scale = 1.0f; 

private:

	//Time in Engine, Real Time Clock
	uint frame_count = 0; 
	float real_time = 0.0f;
	float real_dt = 0.0f; 

	//Game clock
	float time = 0.0f; 
	float dt = 0.0f; 

};

#endif //!__TIME_MANAGER_H__