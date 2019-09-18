#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	uint SetMusicVolume() const
	{
		return Mix_VolumeMusic(masterVolume * (musicVolume * MIX_MAX_VOLUME / 100) / 100);
	}
	uint SetSfxVolume() const {
		return Mix_Volume(-1, masterVolume * (sfxVolume * MIX_MAX_VOLUME / 100) / 100);
	}

	unsigned short masterVolume = 80;
	unsigned short musicVolume = 10;
	unsigned short sfxVolume = 10;

private:

	Mix_Music*			music;
	list<Mix_Chunk*>	fx;
};

#endif // __ModuleAudio_H__