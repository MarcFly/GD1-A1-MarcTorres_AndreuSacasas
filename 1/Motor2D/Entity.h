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
		return true; 
	};

	// Caller before starting loop iteration
	virtual bool Start() { 
		return true; 
	};

	// Called each loop iteration
	virtual bool PreUpdate() { 
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
	virtual bool PostUpdate() { 
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
	
	virtual bool LoadSprites(const pugi::xml_node& sprite_node) {
		return true;
	};

	virtual bool LoadProperties(const pugi::xml_node& property_node) {
		return true;
	}

	virtual void OnCollision(Collider* c1, Collider* c2) {};

	virtual void Movement() {};

public:
	p2SString name;

	SDL_Texture*		graphics = nullptr;
	Animation*			current_animation = nullptr;
	move_state			state = error;
	move_state			last_state = error;
	p2List<Animation*>	animations;
	int					current_anim_size = 0;

	Collider*		collision_box;

	entity_stats	stats;
	float			render_scale;
	fPoint			position;
	bool			flip = false; //false = Right true = Left

	

public: // Functions that go the same for every entity

	void Draw(float dt) {

		current_animation = FindAnimByName(state);

		switch (state)
		{
		case move_state::idle:
			current_anim_size = 7;
			break;
		case move_state::move:
			current_anim_size = 9;
			break;
		case move_state::jump:
			current_anim_size = 5;
			break;
		case move_state::fall:
			current_anim_size = 3;
			break;
		default:
			break;
		}

		
		App->render->Blit(graphics, position.x - current_animation->offset_x, position.y - current_animation->offset_y, &current_animation->frames[current_animation->GetAnimationFrame(dt, current_anim_size)], render_scale, flip);
	}

	void UpdateState();

	void Jump() {
		stats.speed.y += stats.jump_force;
	}

	Animation* FindAnimByName(move_state _state_) {
		p2List_item<Animation*>* ret = animations.start;
		while (ret->data->anim_state != _state_) {
			ret = ret->next;
		}

		return ret->data;
	}
};

#endif // __ENTITY_H__