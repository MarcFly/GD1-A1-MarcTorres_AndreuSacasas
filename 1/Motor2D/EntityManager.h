#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "j1Module.h"
#include "Entity.h"

class Entity;

class EntityManager : public j1Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(const pugi::xml_node& config);

	// Caller before starting loop iteration
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool UpdateTick(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called when triggered
	bool Load(const pugi::xml_node& savegame);

	bool Save(pugi::xml_node& savegame);

private:
	p2List<Entity> entities;

public:
	

};

#endif // __ENTITYMANAGER_H__