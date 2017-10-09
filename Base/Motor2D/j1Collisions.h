#ifndef __j1Collisions_H__
#define __j1Collisions_H__


#define FARLANDS iPoint({5000, 5000})

#include "j1Module.h"
#include "p2Defs.h"
#include "Animation.h"
#include "p2List.h"

class SDL_Rect;
enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_PLAYER,
	//COLLIDER_WALL,
	COLLIDER_HOOK_RING,
	COLLIDER_HOOK_RANGE,
	COLLIDER_GROUND,
	COLLIDER_PLATFORM,

	COLLIDER_MAX
};

struct Collider {
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	bool active = true;

	SDL_Rect rect;

	Collider(SDL_Rect rect_, COLLIDER_TYPE type_, j1Module* callback_ = nullptr) {
		rect = rect_;
		type = type_;
		callback = callback_;
	}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	//Directions ColliderHit(const SDL_Rect& r) const;
	bool CheckCollision(const SDL_Rect& r) const;
};


class j1Collision : public j1Module
{
public:

	j1Collision();
	~j1Collision();

	bool Awake(const pugi::xml_node& config);
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect_, COLLIDER_TYPE type_, j1Module* callback_ = nullptr);
	bool EraseCollider(Collider* collider);
	void DebugDraw();

private:

	p2List<Collider*> colliders;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;

};

#endif // __j1Collisions_H__