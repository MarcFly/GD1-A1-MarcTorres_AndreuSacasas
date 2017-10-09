#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "p2Log.h"

j1Collision::j1Collision()
{
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_HOOK_RING] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_HOOK_RANGE] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = true;

	matrix[COLLIDER_HOOK_RING][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_HOOK_RING][COLLIDER_HOOK_RING] = false;
	matrix[COLLIDER_HOOK_RING][COLLIDER_HOOK_RANGE] = true;
	matrix[COLLIDER_HOOK_RING][COLLIDER_GROUND] = false;
	matrix[COLLIDER_HOOK_RING][COLLIDER_PLATFORM] = false;

	matrix[COLLIDER_HOOK_RANGE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_HOOK_RANGE][COLLIDER_HOOK_RING] = true;
	matrix[COLLIDER_HOOK_RANGE][COLLIDER_HOOK_RANGE] = false;
	matrix[COLLIDER_HOOK_RANGE][COLLIDER_GROUND] = false;
	matrix[COLLIDER_HOOK_RANGE][COLLIDER_PLATFORM] = false;

	matrix[COLLIDER_GROUND][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_GROUND][COLLIDER_HOOK_RING] = false;
	matrix[COLLIDER_GROUND][COLLIDER_HOOK_RANGE] = false;
	matrix[COLLIDER_GROUND][COLLIDER_GROUND] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLATFORM] = false;

	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLATFORM][COLLIDER_HOOK_RING] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_HOOK_RANGE] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_GROUND] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLATFORM] = false;


}

j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	bool ret = true;
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < colliders.count(); ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return ret;
}

bool j1Collision::Update(float dt)
{
	bool ret = true;
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < colliders.count(); ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < colliders.count(); ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			
			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}

			
		}
	}

	DebugDraw();

	return ret;
}

void j1Collision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < colliders.count(); ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_HOOK_RING: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_HOOK_RANGE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_GROUND: // Purple
			App->render->DrawQuad(colliders[i]->rect, 204, 0, 204, alpha);
			break;
		case COLLIDER_PLATFORM:
			App->render->DrawQuad(colliders[i]->rect, 100, 100, 150, alpha);
			break;

		}
	}
}

bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < colliders.count(); ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	colliders.clear();

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect_, COLLIDER_TYPE type_, j1Module* callback_)
{
	Collider* ret = new Collider(rect_,type_,callback_);

	colliders.add(ret);

	ret->to_delete = false;

	return ret;
}

bool j1Collision::EraseCollider(Collider* collider)
{
	
	colliders[colliders.find(collider)]->to_delete = true;
	
	return false;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}