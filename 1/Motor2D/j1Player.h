#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "Entity.h"

class Entity;

class j1Player : public Entity
{
public:

	j1Player(const uint& type, const uint& eid) { this->type = type; this->entity_id = eid; }

	j1Player(const uint& eid, j1Player* template_player) { CopyFromTE(template_player); entity_id = eid; };

	void CopySpecifics(Entity* template_entity);

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
	void MoveUp(float dt);
	void MoveDown(float dt);
	void DoJump(float dt);
	void NoMove(float dt);

	bool LoadSpecificSprites(const pugi::xml_node& sprite_node);

	bool SpecificCleanUp() {
		bool ret = true;
		if(player_life != nullptr)
			App->tex->UnLoad(player_life);
		player_life = nullptr;

		health_source.Clear();
		health_rects.Clear();

		App->tex->UnLoad(god_mode_tex);
		god_mode_source.Clear();
		
		return ret;
	}

	void AddScore(int plus) { score += plus; }
	void AddCoin(int plus) { coins += plus; }

public:

	bool can_jump = true;
	bool is_jumping = false;

	int coins;
	int score;

private:
	SDL_Texture*		player_life = nullptr;
	p2SString			health_source;
	p2DynArray<SDL_Rect> health_rects;

	SDL_Texture*		god_mode_tex = nullptr;
	p2SString			god_mode_source;
	SDL_Rect			god_mode_rect;

private:
	j1Timer				blink;

};


#endif // __j1TEXTURES_H__