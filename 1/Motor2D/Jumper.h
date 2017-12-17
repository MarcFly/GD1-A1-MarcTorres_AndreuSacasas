#ifndef __Jumper_H__
#define __Jumper_H__

#include "Entity.h"

class Entity;

class Jumper : public Entity
{
public:

	Jumper(const uint& type, const uint& eid) { this->type = type; this->entity_id = eid; }

	Jumper(const uint& eid, Jumper* template_jumper) { CopyFromTE(template_jumper); entity_id = eid;};

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

	void MoveRight(float dt);
	void MoveLeft(float dt);

	void NoMove(float dt);

public:

};


#endif // __j1Jumper_H__