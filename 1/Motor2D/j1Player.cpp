#include "j1Player.h"
#include "j1App.h"
#include "j1Input.h"
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

	collision_box->rect = {position.x,position.y, current_animation->GetCurrentFrame().w, current_animation->GetCurrentFrame().h };

	return ret;
}

void j1Player::OnCollision(Collider* c1, Collider* c2, const SDL_Rect& check)
{

}