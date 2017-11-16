#ifndef __Flyer_H__
#define __Flyer_H__

#include "Entity.h"

class Entity;

class Flyer : public Entity
{
public:

	Flyer(const uint& type_, const uint& eid) { type = type_; entity_id = eid; };

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

	// void Dive(float dt);

public:

	float vec_v = rand()%3 + 2; 

};


#endif // __j1Flyer_H__