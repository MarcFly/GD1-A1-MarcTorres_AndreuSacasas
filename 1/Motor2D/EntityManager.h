#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "j1Module.h"
#include "Entity.h"

class Entity;

enum entity_type {
	none = -1,
	player = 0,
	
	max_
};
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

	// Add entity by config
	int AddEntity(const uint& name);
private:
	p2List<Entity*> entities;

	pugi::xml_document	sprites_doc;

public:
	p2SString			tex_folder;

};

#endif // __ENTITYMANAGER_H__