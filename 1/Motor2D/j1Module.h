// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;
class Collider;
struct SDL_Rect;

class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(const pugi::xml_node& config)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Call modules with specific frame required interaction, reach loop iteration
	virtual bool UpdateTick(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}
	
	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// TODDO 2.1: Create new virtual methods to load and save
	//	Load
	virtual bool Load(const pugi::xml_node& savegame) {
		return true;
	}

	//	Save
	virtual bool Save(pugi::xml_node& savegame) {
		return true;
	}

	// Collision Reactor
	virtual bool OnCollision(Collider* c1, Collider* c2, const SDL_Rect& check) { 
		return true;
	};

public:

	p2SString	name;
	bool		active;

	bool		trigger_load;
	bool		trigger_save;

	uint		tick_cap = 0;
	float		dt_sum = 0;
};

#endif // __j1MODULE_H__