#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"

class Entity
{
public:
	Entity() {};

	// Destructor
	virtual ~Entity() {};

	// Called before render is available
	virtual bool Awake(const pugi::xml_node& config) { 
		return true; 
	};

	// Caller before starting loop iteration
	virtual bool Start() { 
		return true; 
	};

	// Called each loop iteration
	virtual bool PreUpdate() { 
		return true; 
	};

	// Called each loop iteration
	virtual bool UpdateTick(float dt) { 
		return true; 
	};

	// Called each loop iteration
	virtual bool Update(float dt) { 
		return true; 
	};

	// Called each loop iteration
	virtual bool PostUpdate() { 
		return true; 
	};

	// Called before quitting
	virtual bool CleanUp() { 
		return true; 
	};

	// Called when triggered
	virtual bool Load(const pugi::xml_node& savegame) {
		return true;
	};

	virtual bool Save(pugi::xml_node& savegame) {
		return true;
	};

};

#endif // __ENTITY_H__