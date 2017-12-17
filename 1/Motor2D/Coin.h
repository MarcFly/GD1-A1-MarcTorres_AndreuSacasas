#ifndef _Coin_H_
#define _Coin_H_

#include "Entity.h"

class Coin : public Entity {
public:
	Coin(const uint& type, const uint& eid) { this->type = type; this->entity_id = eid; }

	Coin(const uint& eid, Coin* template_coin) { CopyFromTE(template_coin); entity_id = eid; };

	bool Start();

	void CopySpecifics(Entity* template_entity);

	virtual void Draw(float dt);

	void OnCollision(Collider* c1, Collider* c2, SDL_Rect& check);
};



#endif //_Coin_H_