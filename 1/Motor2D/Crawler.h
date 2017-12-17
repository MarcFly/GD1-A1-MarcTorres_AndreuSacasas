#ifndef __Crawler_H__
#define __Crawler_H__

#include "Entity.h"

class Entity;

class Crawler : public Entity
{
public:

	Crawler(const uint& type, const uint& eid) { this->type = type; this->entity_id = eid; }

	Crawler(const uint& eid, Crawler* template_crawler) { CopyFromTE(template_crawler); entity_id = eid; };

	// Called before the first frame
	bool Start();

	// Called each loop iteration, for frame specific interactions
	bool UpdateTick(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// OnCollision reaction
	void OnCollision(Collider* c1, Collider* c2, SDL_Rect& check);
	void CorrectCollision(Collider* c1, Collider* c2, SDL_Rect& check);

	void Movement(float dt);

public:

};


#endif // __j1Crawler_H__