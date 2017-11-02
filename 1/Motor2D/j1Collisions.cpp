#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "p2Log.h"

j1Collision::j1Collision() : j1Module()
{
	name.create("collisions");
	for (int i = 0; i < COLLIDER_MAX; i++)
		for (int j = 0; j < COLLIDER_MAX; j++)
			matrix[i][j] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;

	matrix[COLLIDER_PLAYER_AIR][COLLIDER_GROUND] = true;
	
	matrix[COLLIDER_PLAYER_WALL][COLLIDER_GROUND] = true;

	matrix[COLLIDER_HOOK_RANGE][COLLIDER_HOOK_RING] = true;

}

j1Collision::~j1Collision()
{}

bool j1Collision::Awake(pugi::xml_node& config) {
	bool ret = true;

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

bool j1Collision::PreUpdate()
{
	bool ret = true;
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < dynamic_colliders.count(); ++i)
	{
		if (dynamic_colliders[i] != nullptr && dynamic_colliders[i]->to_delete == true)
		{
			delete dynamic_colliders[i];
			dynamic_colliders[i] = nullptr;
		}
	}

	for (uint i = 0; i < passive_colliders.count(); ++i)
	{
		if (passive_colliders[i] != nullptr && passive_colliders[i]->to_delete == true)
		{
			delete passive_colliders[i];
			passive_colliders[i] = nullptr;
		}
	}

	return ret;
}

bool j1Collision::Update(float dt)
{
	bool ret = true;
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < dynamic_colliders.count(); ++i)
	{
		// skip empty colliders
		if (dynamic_colliders[i] == nullptr)
			continue;

		c1 = dynamic_colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < passive_colliders.count(); ++k)
		{
			if (passive_colliders[k] != nullptr && matrix[c1->type][passive_colliders[k]->type] && abs(c1->rect.x - passive_colliders[k]->rect.x) < 200 && abs(c1->rect.y - passive_colliders[k]->rect.y) < 200) {
			// skip empty colliders, colliders that don't interact with active one, colldiers not in range to be a problem (subjective range for now)

				c2 = passive_colliders[k];
			
				SDL_Rect check;
				/*
				if (c2->CheckCollision({
					c1->rect.x + (int)App->player->player.stats.curr_speed,
					c1->rect.y + (int)App->player->player.stats.speed_y,
					c1->rect.w,
					c1->rect.h },
					check) > 0 && abs(check.w) > 0 && abs(check.h) > 0) {

					c1->callback->OnCollision(c1, c2, check);

				}*/
			}
		}
	}

	return ret;
}

void j1Collision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < passive_colliders.count(); ++i)
	{
		if (passive_colliders[i] == nullptr)
			continue;

		switch (passive_colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(passive_colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(passive_colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_HOOK_RING: // red
			App->render->DrawQuad(passive_colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_HOOK_RANGE: // white
			App->render->DrawQuad(passive_colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_GROUND: // Purple
			App->render->DrawQuad(passive_colliders[i]->rect, 204, 0, 204, alpha);
			break;
		case COLLIDER_PLAYER_AIR:
			App->render->DrawQuad(passive_colliders[i]->rect, 100, 100, 150, alpha);
			break;
		case COLLIDER_PLAYER_WALL:
			App->render->DrawQuad(passive_colliders[i]->rect, 100, 100, 150, alpha);
			break;

		}
	}

	for (uint i = 0; i < dynamic_colliders.count(); ++i)
	{
		if (dynamic_colliders[i] == nullptr)
			continue;

		switch (dynamic_colliders[i]->type)
		{
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(dynamic_colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PLAYER_AIR: // green
			App->render->DrawQuad(dynamic_colliders[i]->rect, 0, 255, 50, alpha);
			break;
		case COLLIDER_HOOK_RANGE: // white
			App->render->DrawQuad(dynamic_colliders[i]->rect, 255, 255, 255, alpha);
			break;

		}
	}
}

bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < passive_colliders.count(); ++i)
	{
		if (passive_colliders[i] != nullptr)
		{
			delete passive_colliders[i];
			passive_colliders[i] = nullptr;
		}
	}

	passive_colliders.clear();

	for (uint i = 0; i < dynamic_colliders.count(); ++i)
	{
		if (dynamic_colliders[i] != nullptr)
		{
			delete dynamic_colliders[i];
			dynamic_colliders[i] = nullptr;
		}
	}

	dynamic_colliders.clear();
	rect_list.clear();

	return true;
}

Collider* j1Collision::AddCollider(iPoint pos, COLLIDER_TYPE type_, j1Module* callback_)
{
	SDL_Rect give = GetRectType(type_);
	give.x += pos.x;
	give.y += pos.y;

	Collider* ret = new Collider(give,type_,callback_);

	if (type_ == COLLIDER_PLAYER || type_ == COLLIDER_PLAYER_WALL || type_ == COLLIDER_PLAYER_AIR)
		dynamic_colliders.add(ret);
	else
		passive_colliders.add(ret);

	ret->to_delete = false;

	return ret;
}

bool j1Collision::EraseCollider(Collider* collider)
{
	if(collider->type == COLLIDER_PLAYER || collider->type == COLLIDER_HOOK_RANGE)
		dynamic_colliders[dynamic_colliders.find(collider)]->to_delete = true;

	else
		passive_colliders[passive_colliders.find(collider)]->to_delete = true;
	
	return false;
}

int Collider::CheckCollision(const SDL_Rect& r, SDL_Rect& res_rect) const
{
	SDL_IntersectRect(&rect, &r, &res_rect);
	return (res_rect.w * res_rect.h);
}

SDL_Rect j1Collision::GetRectType(COLLIDER_TYPE type) {

	p2List_item<TypeRect*>* item = rect_list.start;

	while (type != item->data->type)
		item = item->next;

	return item->data->rect;
}