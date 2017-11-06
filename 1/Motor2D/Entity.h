#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Collisions.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;
class Animation;

enum move_state {
	error = -1,
	idle = 0,
	move,
	jump,
	fall,
};

struct entity_stats {
	fPoint accel;
	iPoint max_speed;
	fPoint speed;
	int jump_force;	
};

class Entity
{
public:
	Entity() {};

	// Destructor
	virtual ~Entity() {};

	// Called before render is available
	virtual bool Awake(const pugi::xml_node& config, const pugi::xml_node& sprites) {
		LoadSprites(sprites);

		pugi::xml_node temp = sprites.child("stats");
		LoadProperties(temp);

		return true; 
	};

	// Caller before starting loop iteration
	virtual bool Start() { 
		return true; 
	};

	// Called each loop iteration
	virtual bool PreUpdate(float dt) { 
		App->collisions->LookColl(this, dt);
		return true; 
	};

	// Called each loop iteration
	virtual bool UpdateTick(float dt) { 
		return true; 
	};

	// Called each loop iteration
	virtual bool Update(float dt) { 
		return true; 
	};

	// Called each loop iteration
	virtual bool PostUpdate(float dt) { 
		App->collisions->LookColl(this, dt);
		stats.speed.y += stats.accel.y;
		return true; 
	};

	// Called before quitting
	virtual bool CleanUp() { 
		return true; 
	};

	// Called when triggered
	virtual bool Load(const pugi::xml_node& savegame) {
		return true;
	};

	virtual bool Save(pugi::xml_node& savegame) {
		return true;
	};

	virtual void OnCollision(Collider* c1, Collider* c2, const SDL_Rect& check) {};

	virtual void Movement() {};

public: // Functions that stay the same
	void Draw(float dt);

	void UpdateState();

	bool LoadSprites(const pugi::xml_node& sprite_node);

	bool LoadProperties(const pugi::xml_node& property_node);

public:
	p2SString name;

	SDL_Texture**		graphics = nullptr;
	Animation*			current_animation = nullptr;
	move_state			state = error;
	move_state			last_state = error;
	p2List<Animation*>	animations;
	int					current_anim_size = 0;

	Collider*		collision_box;

	entity_stats	stats;
	float			render_scale;
	iPoint			position;
	bool			flip = false; //false = Right true = Left

	

public: // Short Functions that stay the same for every entity

	
	void Jump() {
		stats.speed.y = stats.jump_force;
	}

	Animation* FindAnimByName(move_state _state_) {
		p2List_item<Animation*>* ret = animations.start;
		while (ret->data->anim_state != _state_) {
			ret = ret->next;
		}

		return ret->data;
	}

	move_state GetState(const p2SString& state_node) {

		if (state_node == "idle") return move_state::idle;
		else if (state_node == "move") return move_state::move;
		else if (state_node == "jump") return move_state::jump;
		else if (state_node == "fall") return move_state::fall;
		return move_state::error;

	}

	
};

#endif // __ENTITY_H__