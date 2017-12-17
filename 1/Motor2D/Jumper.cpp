#include "Jumper.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "EntityManager.h"

bool Jumper::Start()
{
	bool ret = true;

	current_animation = FindAnimByName(idle);
	state = idle;
	HIT_TIMER.Start();

	collision_box = App->collisions->AddCollider(*coll_rect, (COLLIDER_TYPE)(type + COLLIDER_PLAYER), App->entities);
	delete coll_rect;

	return ret;
}

bool Jumper::UpdateTick(float dt)
{
	bool ret = true;

	App->test_ticks++;

	return ret;
}

bool Jumper::Update(float dt)
{
	bool ret = true;

	position.x += stats.speed.x * dt;
	position.y += stats.speed.y * dt;

	collision_box->rect.x = position.x;
	collision_box->rect.y = position.y;

	Movement(dt);

	App->collisions->LookColl(this, dt);

	UpdateState();

	return ret;
}

void Jumper::OnCollision(Collider* c1, Collider* c2, SDL_Rect& check)
{
	if (c2->type == COLLIDER_GROUND)
	{
		CorrectCollision(c1, c2, check);
		c2->CheckCollision(c1->rect, check);
	}
	if (c2->type == COLLIDER_CRAWL_NAV && HIT_TIMER.ReadSec() >= 1)
	{
		this->stats.speed.x *= -1;
		this->stats.max_speed.x *= -1;
		HIT_TIMER.Start();
	}
	else if (c2->type == COLLIDER_DIE)
	{
		App->entities->DestroyEntity(App->entities->FindEntities(type, entity_id));
	}
	else if (c2->type == COLLIDER_PLAYER)
	{
		if (c2->rect.y + c2->rect.h < c1->rect.y + 3) {
			App->entities->DestroyEntity(App->entities->FindEntities(type, entity_id));
			App->entities->FindByColl(c2)->stats.speed *= { 1.1f, -1.2f };
			collision_box->active = false;
		}
		else if (App->entities->FindByColl(c2)->HIT_TIMER.ReadSec() >= 5) {
			if (App->entities->FindByColl(c2)->stats.hp > 0)
				App->entities->FindByColl(c2)->stats.hp -= 1;
			App->entities->FindByColl(c2)->stats.speed *= {-1.0f, -1.0f};
			App->entities->FindByColl(c2)->HIT_TIMER.Start();
			this->stats.speed *= { -1.0f, 1.0f };
		}
	}



	collision_box->rect.x = position.x;
	collision_box->rect.y = position.y;

}

void Jumper::Movement(float dt) {

	// On enemies this will be used for fly and probably later on with jumper and Jumper properly falling
	stats.speed.x = stats.max_speed.x;
}

void Jumper::CorrectCollision(Collider* c1, Collider* c2, SDL_Rect& check)
{
	if ((float)check.w / (float)c1->rect.w > (float)check.h / (float)c1->rect.h)
	{
		if (c1->rect.y < c2->rect.y)
		{
			this->position.y = c2->rect.y - c1->rect.h + 1;
			this->stats.speed.y = 0;
		}
		else if (c1->rect.y >= c2->rect.y)
		{
			this->position.y = c2->rect.y + c2->rect.h + 1;
			this->stats.speed.y = 0;
		}
	}

}