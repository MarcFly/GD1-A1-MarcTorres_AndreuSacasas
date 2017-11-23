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

	collision_box = App->collisions->AddCollider(*coll_rect, (COLLIDER_TYPE)(type + COLLIDER_PLAYER), App->entities);
	delete coll_rect;

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
	if (c2 != nullptr && c1 != nullptr)
	{
		if (c2->type == COLLIDER_GROUND)
		{
			CorrectCollision(c1, c2, check);

		}
		else if (c2->type == COLLIDER_DIE)
		{
			App->entities->DestroyEntity(App->entities->FindEntities(type, entity_id));
		}
		else if (c2->type == COLLIDER_PLAYER)
		{
			if (c2->rect.y + c2->rect.h < c1->rect.y + 3) {
				App->entities->FindByColl(c2)->stats.speed *= { 1.1f, -1.2f };
				collision_box->active = false;
				App->entities->DestroyEntity(App->entities->FindEntities(type, entity_id));
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
}

void Flyer::Movement(float dt) {

	// On enemies this will be used for fly and probably later on with Flyer and Flyer properly falling
	if(position.DistanceTo(App->entities->GetEntity(0)->position) < 1000)
		App->pathfinding->CreateFPath(App->map->WorldToMap(position.x + collision_box->rect.w / 2, position.y + collision_box->rect.h / 2), App->map->WorldToMap(App->entities->GetEntity(0)->position.x, App->entities->GetEntity(0)->position.y));

	check_try = TryPath();
}

bool Flyer::TryPath()
{
	if (App->pathfinding->GetLastPath()->At(0) != nullptr && App->entities->GetEntity(0)->HIT_TIMER.ReadSec() >= 5)
	{

		iPoint next = *App->pathfinding->GetLastPath()->At(0);
		next = App->map->MapToWorld(next.x, next.y);
		next = { next.x + (int)App->map->Maps->tilewidth / 2, next.y + (int)App->map->Maps->tileheight / 2 };
		iPoint present = { position.x + collision_box->rect.w / 2, position.y + collision_box->rect.h / 2 };
		iPoint multiplier = next - present;
		
		if(multiplier.x != 0)
			stats.speed.x = vec_v * (multiplier.x / abs(multiplier.x));
		if (abs(multiplier.x) < 5)
			stats.speed.x = 0;

		if(multiplier.y != 0 )
			stats.speed.y = vec_v * (multiplier.y / abs(multiplier.y));
		if (abs(multiplier.y) < 5)
			stats.speed.y = 0;
	}
	else {
		stats.speed.x = vec_v;
		stats.speed.y = 0;
	}
	return true;
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
	else if ((float)check.w / (float)c1->rect.w <= (float)check.h / (float)c1->rect.h && HIT_TIMER.ReadSec() >= 0.5f)
	{
		this->stats.speed.x *= -1;
		this->vec_v *= -1;
		HIT_TIMER.Start();
	}
}