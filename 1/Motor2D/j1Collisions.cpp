#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1Map.h"
#include "p2Log.h"

j1Collision::j1Collision() : j1Module()
{
	name.create("collisions");
	for (int i = 0; i < COLLIDER_MAX; i++)
		for (int j = 0; j < COLLIDER_MAX; j++)
			matrix[i][j] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DIE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_END] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_GROUND] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_DIE] = true;

	matrix[COLLIDER_GROUND][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_GROUND][COLLIDER_ENEMY] = true;

	matrix[COLLIDER_DIE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DIE][COLLIDER_ENEMY] = true;

	matrix[COLLIDER_END][COLLIDER_PLAYER] = true;
}

j1Collision::~j1Collision()
{}

bool j1Collision::Awake(const pugi::xml_node& config) {
	bool ret = true;

	coll_detect = config.child("settings").attribute("coll_detect").as_uint();

	pugi::xml_node local = config.child("collider");
	while (local.attribute("type").as_string() != "") {
		TypeRect* item = new TypeRect;
		item->type = (COLLIDER_TYPE)local.attribute("type").as_int();
		item->rect = {
			local.attribute("x").as_int(),
			local.attribute("y").as_int(),
			local.attribute("w").as_int(),
			local.attribute("h").as_int()
		};

		rect_list.add(item);
		local = local.next_sibling("collider");
	}

	return ret;
}

bool j1Collision::PreUpdate(float dt)
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
	/*Collider* c1;
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
			if (colliders[k] != nullptr && matrix[c1->type][colliders[k]->type] && abs(c1->rect.x - colliders[k]->rect.x) < 100 && abs(c1->rect.y - colliders[k]->rect.y) < 100) {
			// skip empty colliders, colliders that don't interact with active one, colldiers not in range to be a problem (subjective range for now)

				c2 = colliders[k];
			
				SDL_Rect check;
				/*
				if (c2->CheckCollision({
					c1->rect.x + (int)App->player->player.stats.curr_speed,
					c1->rect.y + (int)App->player->player.stats.speed_y,
					c1->rect.w,
					c1->rect.h },
					check) > 0 && abs(check.w) > 0 && abs(check.h) > 0) {

					c1->callback->OnCollision(c1, c2, check);

				}
			}
		}
	}
	*/
	return ret;
}

void j1Collision::LookColl(Entity* entity, float dt) {
	
	Collider* c1 = entity->collision_box;
	Collider* c2;

	for (uint k = 0; k < colliders.count(); ++k)
	{
		if (colliders[k] != nullptr && matrix[c1->type][colliders[k]->type] && abs(c1->rect.x - colliders[k]->rect.x) < coll_detect && abs(c1->rect.y - colliders[k]->rect.y) < coll_detect) {
			// skip empty colliders, colliders that don't interact with active one, colldiers not in range to be a problem (subjective range for now)
			c2 = colliders[k];

			if (c1 != c2) {
				SDL_Rect check;

				if (c2->CheckCollision({
				c1->rect.x + (int)(entity->stats.speed.x * dt),
				c1->rect.y + (int)(entity->stats.speed.y * dt),
				c1->rect.w,
				c1->rect.h },
				check) > 0 && check.w > 0 && check.h > 0) {

					entity->OnCollision(c1, c2, check);

				}
			}
		}
	}

	entity->UpdateState();

}

void j1Collision::DebugDraw()
{
	if (App->map->debug_draw == false)
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
		case COLLIDER_GROUND: // Purple
			App->render->DrawQuad(colliders[i]->rect, 204, 0, 204, alpha);
			break;
		case COLLIDER_DIE:
			break;
		case COLLIDER_END:
			break;
		case COLLIDER_ENEMY:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
				break;
		

		}
	}
}

bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	CleanColliders();

	rect_list.clear();

	return true;
}

bool j1Collision::CleanColliders(){
	bool ret = true;
	p2List_item<Collider*>* item = colliders.start;
	while (item != NULL)
	{
		if(item->data != nullptr)
			delete item->data;
		item->data = nullptr;
		item = item->next;
	}

	colliders.clear();

	return ret;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type_, j1Module* callback_)
{
	Collider* ret = new Collider(rect,type_,callback_);

	colliders.add(ret);

	ret->to_delete = false;

	return ret;
}

bool j1Collision::EraseCollider(Collider* collider)
{
	colliders[colliders.find(collider)]->to_delete = true;
	
	return false;
}

int Collider::CheckCollision(const SDL_Rect& r, SDL_Rect& res_rect) const
{
	SDL_IntersectRect(&this->rect, &r, &res_rect);
	return (res_rect.w * res_rect.h);
}
