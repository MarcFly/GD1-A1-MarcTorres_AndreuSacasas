#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;


enum player_state {
	error = -1,
	idle = 0,
	move,
	running,
	air,
	jumpsquat,
	landing,
	hooking,
	hooked
};

struct player_stats {
	float accel;
	float max_speed;
	float gravity;
	int jump_force;
	float hook_range;
	int hook_time;
	float aerial_drift;
	float curr_speed;
};

struct player_char
{
	SDL_Texture*		graphics;
	Animation*			current_animation = nullptr;
	player_state		state;
	p2List<Animation*>	animations;

	p2SString		name;
	player_stats	stats;
	iPoint			offset;
	float			render_scale;
	fPoint			position;
	bool			direction = true; //false = Left true = Right
	


	Animation* FindAnimByName(player_state _state_) {
		p2List_item<Animation*>* ret = animations.start;
		while (ret->data->anim_state != _state_) {
			ret = ret->next;
		}

		return ret->data;
	}

	~player_char() {
		App->tex->UnLoad(main_player.graphics);
		delete current_animatin;
		animations.clear();
	}
};


class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	int timer = 0;

	bool Awake(const pugi::xml_node& config);
	bool Start();

	bool Update(float dt);
	bool CleanUp();
	void OnCollision();

	bool Load(const pugi::xml_node& config);
	bool Save(const pugi::xml_node& config);


	bool LoadSprites(const pugi::xml_node& sprite_node);
	bool LoadProperties(const pugi::xml_node& property_node);
public:

	void Jump();

	void SlideStart();

	void SlideEnd();

	void Movement();

	void Hook();

	player_state Get_State(const p2SString& state_node);

public:

	player_char main_player;
	
	pugi::xml_document sprites;
};

#endif