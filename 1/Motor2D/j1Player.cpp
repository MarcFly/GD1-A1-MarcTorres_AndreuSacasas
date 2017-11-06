#include "j1Player.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"

j1Player::j1Player()
{
	name.create("player");
}

bool j1Player::Start()
{
	bool ret = true;

	current_animation = FindAnimByName(idle);
	state = idle;

	return ret;
}

bool j1Player::CleanUp()
{
	bool ret = true;

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

	
	// Camera movement Inputs
	// TODO 10.6: Make the camera movement independent of framerate

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1200 * dt;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1200 * dt;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1200 * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1200 * dt;

	position.x += stats.speed.x * dt;
	position.y += stats.speed.y * dt;

	collision_box->rect = {position.x,position.y, current_animation->GetCurrentFrame().w, current_animation->GetCurrentFrame().h };

	Movement(dt);

	//App->collisions->LookColl(this, dt);

	LOG("Player POS %i %i", position.x, position.y);

	return ret;
}

void j1Player::OnCollision(Collider* c1, Collider* c2, const SDL_Rect& check)
{
	if (c2->type == COLLIDER_GROUND)
	{
		if (check.w > check.h)
		{
			if (c1->rect.y > c2->rect.y)
			{
				this->position.y += (check.h + 1);
				this->stats.speed.y = 0;
			}
			else
			{
				this->position.y -= (check.h + 1);
				this->stats.speed.y = 0;
			}
		}
		else
		{
			if (c1->rect.x > c2->rect.x)
			{
				this->position.x += (check.w + 1);
				this->stats.speed.x = 0;
			}
			else
			{
				this->position.x -= (check.w + 1);
				this->stats.speed.x = 0;
			}
		}
	}
	else if (c2->type == COLLIDER_END)
	{

	}
	else if (c2->type == COLLIDER_DIE)
	{

	}
	else if (c2->type == COLLIDER_ENTITY)
	{

	}
}

void j1Player::Movement(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		if (stats.speed.x > 0 || (stats.speed.x <= 0 && abs(stats.speed.x) + stats.accel.x <= stats.max_speed.x))
			stats.speed.x -= stats.accel.x;

		else
			stats.speed.x = -stats.max_speed.x;

		if (state == move_state::move)
			can_jump = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

		if (stats.speed.x < 0 || (stats.speed.x >= 0 && abs(stats.speed.x) + stats.accel.x <= stats.max_speed.x))
			stats.speed.x += stats.accel.x;
		else
			stats.speed.x = stats.max_speed.x;

		if (state == move_state::move)
			can_jump = true;
	}
	else
	{
		if (stats.speed.x == 0 && stats.speed.y == 0 && !can_jump)
			can_jump = true;

		if (stats.speed.x < 0) {

			if (stats.speed.x + stats.accel.x / 2 >= 0)
				stats.speed.x = 0;
			else
				stats.speed.x += stats.accel.x / 2;
		}
		else if (stats.speed.x > 0) {
			if (stats.speed.x - stats.accel.x / 2 <= 0)
				stats.speed.x = 0;
			else
				stats.speed.x -= stats.accel.x / 2;
		}
	}
	if ((state == fall) && stats.speed.y <= stats.max_speed.y + stats.accel.y)
	{
		stats.speed.y += stats.accel.y ;

	}
	else {

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && can_jump == true) {
			if (!is_jumping)
				Jump();

			is_jumping = true;
			if (stats.speed.y < -6)
				stats.speed.y /= 1.07;

			else
			{
				can_jump = false;
			}
		}

		else if (is_jumping == true)
		{
			can_jump = false;
			is_jumping = false;
		}

		
	}
	// Draw everything --------------------------------------

}

