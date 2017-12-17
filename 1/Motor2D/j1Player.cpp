#include "j1Player.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "EntityManager.h"

void j1Player::CopySpecifics(Entity* template_ent) {

	stats.hp = 4;
	j1Player* test = (j1Player*)template_ent;

	health_source.create(test->health_source.GetString());
	god_mode_source.create(test->god_mode_source.GetString());

	for (int i = 0; i < test->health_rects.Count(); i++)
	{
		health_rects.PushBack(test->health_rects[i]);
	}

	player_life = test->player_life;
	god_mode_tex = test->god_mode_tex;

	god_mode_rect = test->god_mode_rect;
}

bool j1Player::Start()
{
	bool ret = true;

	current_animation = FindAnimByName(idle);
	state = idle;
	HIT_TIMER.Start();
	blink.Start();
	player_life = App->tex->Load(health_source.GetString());
	god_mode_tex = App->tex->Load(god_mode_source.GetString());

	collision_box = App->collisions->AddCollider(*coll_rect, (COLLIDER_TYPE)(type + COLLIDER_PLAYER), App->entities);
	delete coll_rect;

	return ret;
}

bool j1Player::UpdateTick(float dt)
{
	bool ret = true;

	App->test_ticks++;

	return ret;
}

bool j1Player::Update(float dt)
{
	bool ret = true;

	position.x += stats.speed.x * dt;
	position.y += stats.speed.y * dt;
		
	collision_box->rect.x = position.x;
	collision_box->rect.y = position.y;

	Movement(dt);
	
	App->collisions->LookColl(this, dt);
	UpdateState();

	if (!App->scene->god_mode)
		App->render->camera.x = -position.x + 300;
	else
		App->render->camera.x = -position.x + 300 + App->render->offset_camera;

	return ret;
}

void j1Player::Draw(float dt)
{
	if (this != nullptr) {
		current_animation = FindAnimByName(state);

		if (HIT_TIMER.ReadSec() < 1 && blink.ReadSec() > 0.3)
			blink.Start();

		if (HIT_TIMER.ReadSec() > 1 || blink.ReadSec() < 0.2)
			App->render->Blit(*graphics,
				position.x - current_animation->offset_x,
				position.y - current_animation->offset_y,
				&current_animation->frames[current_animation->GetAnimationFrame(dt, current_animation->frames.Count())],
				current_animation->speed,
				0.0,
				render_scale,
				flip);

		App->render->Blit(player_life, -App->render->camera.x + 900, -App->render->camera.y + 30, &health_rects[this->stats.hp], 1.0f, 0.0, 1);

		if (App->scene->god_mode)
			App->render->Blit(god_mode_tex, -App->render->camera.x + 50, -App->render->camera.y + 30, &god_mode_rect, 1.0f, 0.0f, 0.8f);
	}
}

void j1Player::OnCollision(Collider* c1, Collider* c2, SDL_Rect& check)
{
	if (c2->type == COLLIDER_GROUND)
	{
			CorrectCollision(c1, c2, check);
			c2->CheckCollision(c1->rect, check);		
	}
	else if (c2->type == COLLIDER_END)
	{
		App->scene->change_map = true;
	}
	else if (c2->type == COLLIDER_DIE)
	{
		position.x = App->map->Maps->start_pos.x;
		position.y = App->map->Maps->start_pos.y;
		if (stats.hp > 0 && App->scene->god_mode == false)
			stats.hp -= 1;
	}
	else if (c2->type == COLLIDER_CRAWLER)
	{
		if (c1->rect.y + c1->rect.h < c2->rect.y + 3) {
			if (App->entities->FindByColl(c2) != nullptr) {
				App->entities->DestroyEntity(App->entities->FindEntities(App->entities->FindByColl(c2)->type, App->entities->FindByColl(c2)->entity_id));
				c2->active = false;
				stats.speed *= { 1.1f, -1.2f };
			}
		}
		else if(HIT_TIMER.ReadSec() >= 1){
			if (stats.hp > 0 && App->scene->god_mode == false)
				stats.hp -= 1;
			App->entities->FindByColl(c2)->stats.speed *= {-1.0f, -1.0f};
			stats.speed *= { -2.0f, -1.0f };
			HIT_TIMER.Start();
		}
	}
	else if (c2->type == COLLIDER_FLYER)
	{
		if (c1->rect.y + c1->rect.h < c2->rect.y + 3) {
			App->entities->DestroyEntity(App->entities->FindEntities(App->entities->FindByColl(c2)->type, App->entities->FindByColl(c2)->entity_id));
			c2->active = false;
			stats.speed *= { 1.1f, -1.2f };
		}
		else if (HIT_TIMER.ReadSec() >= 1) {
			if (stats.hp > 0 && App->scene->god_mode == false)
				stats.hp -= 1;
			App->entities->FindByColl(c2)->stats.speed *= {-1.0f, -1.0f};
			stats.speed *= { -2.0f, -1.0f };
			HIT_TIMER.Start();
		}
	}
	
	
	collision_box->rect.x = position.x;
	collision_box->rect.y = position.y;

}

void j1Player::Movement(float dt) {


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		MoveLeft(dt);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		MoveRight(dt);
	}
	else
	{
		NoMove(dt);
	}


	if (!App->scene->god_mode)
	{
		if ((App->input->GetKey(SDL_SCANCODE_SPACE) == (KEY_REPEAT || KEY_DOWN) || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && can_jump == true) {
			DoJump(dt);
		}

		else if (is_jumping == true)
		{
			can_jump = false;
			is_jumping = false;
		}
	}

	else
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			MoveDown(dt);
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			MoveUp(dt);
		}
	}

}

void j1Player::MoveLeft(float dt){
	if (abs(stats.speed.x) + stats.accel.x * dt <= stats.max_speed.x && stats.speed.x < 0)
		stats.speed.x -= stats.accel.x * dt;
	else if (stats.speed.x >= 0)
		stats.speed.x -= stats.accel.x * 2 * dt;
	else
		stats.speed.x = -stats.max_speed.x;

	if (state == move_state::move)
		can_jump = true;
}

void j1Player::MoveUp(float dt)
{
	
	if (abs(stats.speed.x) + stats.accel.x * dt <= stats.max_speed.y)
		stats.speed.y += stats.accel.y * dt;
	else if (stats.speed.x >= 0)
		stats.speed.y += stats.accel.y * 2 * dt;
	else
		stats.speed.y = stats.max_speed.x;
}

void j1Player::MoveDown(float dt)
{
	if (abs(stats.speed.x) + stats.accel.x * dt <= stats.max_speed.y && stats.speed.y < 0)
		stats.speed.y -= stats.accel.y * dt;
	else if (stats.speed.x >= 0)
		stats.speed.y -= stats.accel.y * 2 * dt;
	else
		stats.speed.y = -stats.max_speed.x;
}

void j1Player::MoveRight(float dt) {
	if (abs(stats.speed.x) + stats.accel.x * dt <= stats.max_speed.x)
		stats.speed.x += stats.accel.x * dt;
	else if (stats.speed.x <= 0)
		stats.speed.x += stats.accel.x * 2 * dt;
	else
		stats.speed.x = stats.max_speed.x;

	if (state == move_state::move)
		can_jump = true;
}

void j1Player::NoMove(float dt) {
	if (stats.speed.x == 0 && stats.speed.y == 0 && !can_jump)
		can_jump = true;

	if (stats.speed.x < 0) {

		if (stats.speed.x + stats.accel.x * dt >= 0)
			stats.speed.x = 0;
		else
			stats.speed.x += stats.accel.x * 2 * dt;
	}
	else if (stats.speed.x > 0) {
		if (stats.speed.x - stats.accel.x * 2 * dt <= 0)
			stats.speed.x = 0;
		else
			stats.speed.x -= stats.accel.x * 2 * dt;
	}
}

bool j1Player::LoadSpecificSprites(const pugi::xml_node & sprite_node)
{
	bool ret = true;
	// While no UI module

	pugi::xml_node health = sprite_node.child("ui").child("health");
	pugi::xml_node rect = health.child("rect");
	for (int i = 0; rect.attribute("x").as_string() != ""; i++)
	{
		health_rects.PushBack({
			rect.attribute("x").as_int(),
			rect.attribute("y").as_int(),
			health.attribute("w").as_int(),
			health.attribute("h").as_int(),
		});

		rect = rect.next_sibling("rect");
	}

	health_source.create(health.attribute("source").as_string());

	god_mode_rect = { sprite_node.child("ui").child("god_mode").attribute("x").as_int(),
		sprite_node.child("ui").child("god_mode").attribute("y").as_int(),
		sprite_node.child("ui").child("god_mode").attribute("w").as_int(),
		sprite_node.child("ui").child("god_mode").attribute("h").as_int() };
	
	god_mode_source.create(sprite_node.child("ui").child("god_mode").attribute("source").as_string());

	return ret;
}

void j1Player::DoJump(float dt) {
	if (!is_jumping && !(stats.speed.y > stats.accel.y * 5))
		stats.speed.y = stats.jump_force;

	is_jumping = true;

	if (stats.speed.y < 0)
		stats.speed.y /= 1.07;

	else
	{
		can_jump = false;
		is_jumping = false;
	}
}

void j1Player::CorrectCollision(Collider* c1, Collider* c2, SDL_Rect& check)
{
	if ((float)check.w / (float)c1->rect.w > (float)check.h / (float)c1->rect.h)
	{
		if (c1->rect.y < c2->rect.y && stats.speed.y > 0)
		{
			this->position.y = c2->rect.y - c1->rect.h;
			this->stats.speed.y = 0;
			this->can_jump = true;
		}
		else if (c1->rect.y >= c2->rect.y)
		{
			this->position.y = c2->rect.y + c2->rect.h;
			this->stats.speed.y = 0;
		}
	}
	if ((float)check.h / (float)c1->rect.h  > (float)check.w / (float)c1->rect.w && check.h > this->stats.accel.y * 10)
	{
		if (c1->rect.x > c2->rect.x)
		{
			this->position.x = c2->rect.x + c2->rect.w;
			this->stats.speed.x = 0;
			can_jump = true;
		}
		else if (c1->rect.x <= c2->rect.x)
		{
			this->position.x = c2->rect.x - c1->rect.w;
			this->stats.speed.x = 0;
			can_jump = true;
		}
	}

	if (abs(this->stats.speed.y) > 0 && abs(this->stats.speed.x) > this->stats.accel.x * 10)
	{
		if ((this->position.x + this->stats.speed.x - c2->rect.x) < (this->position.x - c2->rect.x) && check.h > 0 && this->stats.speed.y)
		{
			if (c1->rect.x > c2->rect.x)
			{
				this->position.x = c2->rect.x + c2->rect.w;
				this->stats.speed.x = 0;
				can_jump = true;
			}
			else if (c1->rect.x <= c2->rect.x)
			{
				this->position.x = c2->rect.x - c1->rect.w;
				this->stats.speed.x = 0;
				can_jump = true;
			}
		}
	}
}