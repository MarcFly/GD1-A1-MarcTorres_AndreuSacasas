#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "j1Module.h"
#include "Entity.h"
#include "j1App.h"
#include "j1Textures.h"

class Entity;

enum entity_type {
	none = -1,
	player = 0,
	
	max_
};

struct tex {
	p2SString		source;
	SDL_Texture*	texture;

	tex() {}

	tex(p2SString path) {
		source = path;
	}

	~tex() {
		source.Clear();
		App->tex->UnLoad(texture);
	}

	bool operator ==(tex comp) const{
		return (comp.source == source);
	}
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
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool UpdateTick(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Called to delete entities
	bool CleanEntities();

	bool LoadEntities();

	// Called when triggered
	bool Load(const pugi::xml_node& savegame);

	bool Save(pugi::xml_node& savegame);

	void Draw(float dt);

	// Add entity by config
	int AddEntity(const uint& name, const uint& eid);

	int FindEntities(const uint& type);

private:
	p2List<Entity*>			entities;
	

public:
	pugi::xml_document		sprites_doc;
	p2SString			tex_folder;
	p2List<tex>			texs;

};

#endif // __ENTITYMANAGER_H__