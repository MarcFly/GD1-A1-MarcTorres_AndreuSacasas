#include "Crawler.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "EntityManager.h"

bool Crawler::Start()
{
	bool ret = true;

	current_animation = FindAnimByName(idle);
	state = idle;

	return ret;
}

bool Crawler::UpdateTick(float dt)
{
	bool ret = true;

	App->test_ticks++;

	return ret;
}

bool Crawler::Update(float dt)
{
	bool ret = true;

	position.x += stats.speed.x * dt;
	position.y += stats.speed.y * dt;

	collision_box->rect.x = position.x;
	collision_box->rect.y = position.y;

	Movement(dt);

	App->collisions->LookColl(this, dt);


	return ret;
}

void Crawler::OnCollision(Collider* c1, Collider* c2, SDL_Rect& check)
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
			App->entities->GetEntity(App->entities->FindEntities(type, entity_id))->stats.speed *= { 1.0F, -1.0f };
		}
		else if(App->entities->GetEntity(App->entities->FindEntities(type, entity_id))->HIT_TIMER.ReadSec() >= 5) {
			App->entities->FindByColl(c2)->stats.hp -= 1;
			App->entities->FindByColl(c2)->stats.speed *= {-1.0f, -1.0f};
			this->stats.speed *= { -1.0f,-1.0f };
			App->entities->GetEntity(App->entities->FindEntities(type, entity_id))->HIT_TIMER.Start();
		}
	}



	collision_box->rect.x = position.x;
	collision_box->rect.y = position.y;

}

void Crawler::Movement(float dt) {

	// On enemies this will be used for fly and probably later on with jumper and crawler properly falling
	stats.speed.x = stats.max_speed.x;
}

void Crawler::CorrectCollision(Collider* c1, Collider* c2, SDL_Rect& check)
{
	if ((float)check.w / (float)c1->rect.w > (float)check.h / (float)c1->rect.h)
	{
		if (c1->rect.y < c2->rect.y)
		{
			this->position.y = c2->rect.y - c1->rect.h;
			this->stats.speed.y = 0;
		}
		else if (c1->rect.y >= c2->rect.y)
		{
			this->position.y = c2->rect.y + c2->rect.h;
			this->stats.speed.y = 0;
		}
	}
	if ((float)check.h / (float)c1->rect.h  > (float)check.w / (float)c1->rect.w)
	{
		if (c1->rect.x > c2->rect.x)
		{
			this->position.x = c2->rect.x + c2->rect.w;
			this->stats.speed.x *= -1;
			this->stats.max_speed.x *= -1;
		}
		else if (c1->rect.x <= c2->rect.x)
		{
			this->position.x = c2->rect.x - c1->rect.w;
			this->stats.speed.x *= -1;
			this->stats.max_speed.x *= -1;
		}
	}

	if (abs(this->stats.speed.y) > 0 && abs(this->stats.speed.x) > this->stats.accel.x * 10)
	{
		if ((this->position.x + this->stats.speed.x - c2->rect.x) < (this->position.x - c2->rect.x) && check.h > 0 && this->stats.speed.y)
		{
			if (c1->rect.x > c2->rect.x)
			{
				this->position.x = c2->rect.x + c2->rect.w;
				this->stats.speed.x *= -1;
				this->stats.max_speed.x *= -1;
			}
			else if (c1->rect.x <= c2->rect.x)
			{
				this->position.x = c2->rect.x - c1->rect.w;
				this->stats.speed.x *= -1;
				this->stats.max_speed.x *= -1;
			}
		}
	}
}