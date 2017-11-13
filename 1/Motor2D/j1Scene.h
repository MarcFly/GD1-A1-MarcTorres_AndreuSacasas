#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(const pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void LoadNextMap();

	int curr_map;
	p2List<p2SString*> Map_list;

	void GodMode(float dt);

	void NotGodMode(float dt);

private:
	SDL_Texture* img;

	bool god_mode = false;
	
};

#endif // __j1SCENE_H__