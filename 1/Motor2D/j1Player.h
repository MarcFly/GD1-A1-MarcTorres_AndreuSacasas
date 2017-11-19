#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "Entity.h"

class Entity;

class j1Player : public Entity
{
public:

	j1Player(const uint& type_, const uint& eid) { type = type_; entity_id = eid; };

	// Called before the first frame
	bool Start();

	// Called each loop iteration, for frame specific interactions
	bool UpdateTick(float dt);

	// Called each loop iteration
	bool Update(float dt);

	virtual void Draw(float dt);

	// OnCollision reaction
	void OnCollision(Collider* c1, Collider* c2, SDL_Rect& check);
	void CorrectCollision(Collider* c1, Collider* c2, SDL_Rect& check);

	void Movement(float dt);

	void MoveRight(float dt);
	void MoveLeft(float dt);
	void DoJump(float dt);
	void NoMove(float dt);

	bool LoadSpecificSprites(const pugi::xml_node& sprite_node);
public:

	bool can_jump = true;
	bool is_jumping = false;

	SDL_Texture*		player_life = nullptr;
	p2SString			health_source;
	p2DynArray<SDL_Rect> health_rects;
};


#endif // __j1TEXTURES_H__