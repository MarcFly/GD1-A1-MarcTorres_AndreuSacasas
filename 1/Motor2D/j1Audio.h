#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f



struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(const pugi::xml_node& config);

	// Called each loop iteration
	bool PreUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = 2.0f);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	// Called when triggered
	bool Load(const pugi::xml_node& savegame);

	bool Save(pugi::xml_node& savegame);

private:

	_Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;


	int					master_volume;
	int					music_volume;
	int					fx_volume;

public:
	void Increase_Master()
	{
		master_volume += 2;
		if (master_volume > 128)
			master_volume = 128;

		fx_volume += 2;
		if (fx_volume > 128)
			fx_volume = 128;
	}
	void Decrease_Master()
	{
		master_volume -= 2;
		if (master_volume < 0)
			master_volume = 0;

		fx_volume -= 2;
		if (fx_volume < 0)
			fx_volume = 0;

	}
	void SetMaster(int volume) { master_volume = volume; }
};

#endif // __j1AUDIO_H__