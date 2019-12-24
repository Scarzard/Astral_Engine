#include "Application.h"
#include "Globals.h"
#include "TimeManager.h"

TimeManager::TimeManager(bool start_enabled) : Module(start_enabled)
{
}

TimeManager::~TimeManager()
{
}


update_status TimeManager::PostUpdate(float dt)
{
	frame_count++;

	real_dt = App->GetDT();
	real_time += real_dt;

	if (!game_paused)
		time = real_time - started_play_time - time_in_pause;
	else
		time_in_pause += real_dt;

	return UPDATE_CONTINUE;
}

bool TimeManager::CleanUp()
{
	return true;
}

void TimeManager::ResetGameTimer()
{
	time = 0.0f;
	dt = 0.0f;
	time_in_pause = 0.0f;
	started_play_time = 0.0f;
}

uint TimeManager::GetFrameCount() const
{
	return frame_count;
}

float TimeManager::GetRealTimeClock() const
{
	return real_time;
}

float TimeManager::GetRealDT() const
{
	return real_dt;
}

float TimeManager::GetGameTime() const
{
	return time;
}

float TimeManager::GetTimeScale() const
{
	return time_scale;
}

float TimeManager::GetGameDT() const
{
	return dt;
}