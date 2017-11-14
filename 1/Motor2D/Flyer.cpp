#include "Flyer.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "EntityManager.h"
#include "j1Pathfinding.h"

bool Flyer::Start()
{
	bool ret = true;

	current_animation = FindAnimByName(idle);
	state = idle;
	HIT_TIMER.Start();

	vec_v = sqrt(stats.max_speed.x * stats.max_speed.x + stats.max_speed.y * stats.max_speed.y);

	return ret;
}

bool Flyer::UpdateTick(float dt)
{
	bool ret = true;

	App->test_ticks++;

	return ret;
}

bool Flyer::Update(float dt)
{
	bool ret = true;

	stats.speed.y -= stats.accel.y;

	position.x += stats.speed.x * dt;
	position.y += stats.speed.y * dt;

	collision_box->rect.x = position.x;
	collision_box->rect.y = position.y;

	Movement(dt);

	App->collisions->LookColl(this, dt);

	UpdateState();

	return ret;
}

void Flyer::OnCollision(Collider* c1, Collider* c2, SDL_Rect& check)
{
	if (c2->type == COLLIDER_GROUND)
	{
		CorrectCollision(c1, c2, check);
		c2->CheckCollision(c1->rect, check);
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
			App->entities->FindByColl(c2)->stats.hp -= 1;
			App->entities->FindByColl(c2)->stats.speed *= {-1.0f, -1.0f};
			App->entities->FindByColl(c2)->HIT_TIMER.Start();
			this->stats.speed *= { -1.0f, 1.0f };
		}
	}



	collision_box->rect.x = position.x;
	collision_box->rect.y = position.y;

	

}

void Flyer::Movement(float dt) {

	// On enemies this will be used for fly and probably later on with Flyer and Flyer properly falling
	if(position.DistanceTo(App->entities->GetEntity(0)->position) < 1000)
		App->pathfinding->CreateFPath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->entities->GetEntity(0)->position.x, App->entities->GetEntity(0)->position.y));

	if (App->pathfinding->GetLastPath()->At(0) != nullptr && App->entities->GetEntity(0)->HIT_TIMER.ReadSec() >= 5)
	{

		iPoint next = *App->pathfinding->GetLastPath()->At(0);
		iPoint present = App->map->WorldToMap(position.x, position.y);
		iPoint multiplier = next - App->map->WorldToMap(position.x, position.y);

		stats.speed.x = vec_v * multiplier.x;
		stats.speed.y = vec_v * multiplier.y;

	}
	else {
		stats.speed.x = vec_v;
		stats.speed.y = 0;
	}
}

void Flyer::CorrectCollision(Collider* c1, Collider* c2, SDL_Rect& check)
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