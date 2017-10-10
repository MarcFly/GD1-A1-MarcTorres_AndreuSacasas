#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Collisions.h"
#include "j1App.h"

struct SDL_Texture;
struct Collider;
class Animation;

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
	SDL_Texture*		graphics = nullptr;
	Animation*			current_animation = nullptr;
	player_state		state;
	p2List<Animation*>	animations;

	Collider*		player;

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
		App->tex->UnLoad(graphics);
		delete current_animation;
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

	void Movement();

	void Hook();

	//void OnCollision(Collider* bodyA, Collider* bodyB);

	player_state Get_State(const p2SString& state_node);

public:

	player_char player;
	
	pugi::xml_document sprites;

	pugi::xml_node local_node;
};

#endif