#include "p2defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "SDL/include/SDL_timer.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1Map.h"
#include "list"

#include<stdio.h>
#include<math.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");

}

j1Player::~j1Player()
{}

// Awake, actually load things?
bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	local_node = config;

	return ret;
}
// Load assets
bool j1Player::Start()
{
	bool ret = true;

	pugi::xml_parse_result result = sprites.load_file("sprites.xml");

	if (result == NULL) { //Check that it loaded
		LOG("Could not load sprite xml file player_sprites.xml. pugi error: %s", result.description());
		ret = false;
	}
	else {
		LoadProperties(sprites.child("sprites").child("player"));
	}

	// Inicializar lo necesario del jugador, crear los personajes en el mapa	
	player.state = idle;

	return ret;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	/*App->audio->Unload(audio_shot);

	App->fonts->UnLoad(font_score);*/

	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	bool ret = true;

	player.position.x += player.stats.curr_speed;
	player.position.y += player.stats.speed_y;

	player.collision_box->rect.x = player.position.x;
	player.collision_box->rect.y = player.position.y;

	player.air_box->rect.x = player.position.x;
	player.air_box->rect.y = player.position.y - 1;

	player.wall_box->rect.x = player.position.x - 1;
	player.wall_box->rect.y = player.position.y;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		player.flip = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		player.flip = true;
	}

	//Move player

	/*if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT){
	if (player.current_animation->anim_state == crawl) {

	}
	else {

	}
	}

	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

	}*/


	// Draw everything --------------------------------------

	UpdateState();

	Movement();



	return true;

}

void j1Player::Draw(float dt) {

	player.current_animation = player.FindAnimByName(player.state);

	switch (player.state)
	{
	case player_state::idle:
		player.current_anim_size = 7;
		break;
	case player_state::move:
		player.current_anim_size = 9;
		break;
	case player_state::jump:
		player.current_anim_size = 5;
		break;
	case player_state::fall:
		player.current_anim_size = 3;
		break;
	default:
		break;
	}

	if (player.flip)
		App->render->FlipBlit(player.graphics, player.position.x - player.current_animation->offset_x, player.position.y - player.current_animation->offset_y, &player.current_animation->frames[player.current_animation->GetAnimationFrame(dt, player.current_anim_size)], player.render_scale);
	else
		App->render->Blit(player.graphics, player.position.x- player.current_animation->offset_x, player.position.y - player.current_animation->offset_y, &player.current_animation->frames[player.current_animation->GetAnimationFrame(dt, player.current_anim_size)], player.render_scale);
}

void j1Player::UpdateState() {


	if (player.stats.speed_y < 0)
		player.state = jump;
	else if (player.stats.speed_y > 0)
		player.state = fall;
	else if (abs(player.stats.curr_speed) > player.stats.accel)
		player.state = move;
	else if (player.stats.speed_y == 0 && player.stats.curr_speed == 0)
		player.state = idle;

	LOG("Player State %d", player.state);
}


void j1Player::Movement() {

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		if (player.stats.curr_speed > 0 || (player.stats.curr_speed <= 0 && abs(player.stats.curr_speed) + player.stats.accel <= player.stats.max_speed.x))
			player.stats.curr_speed -= player.stats.accel;

		else
			player.stats.curr_speed = -player.stats.max_speed.x;

		if (player.state == player_state::move)
			can_jump = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

		if (player.stats.curr_speed < 0 || (player.stats.curr_speed >= 0 && abs(player.stats.curr_speed) + player.stats.accel <= player.stats.max_speed.x))
			player.stats.curr_speed += player.stats.accel;
		else
			player.stats.curr_speed = player.stats.max_speed.x;

		if (player.state == player_state::move)
			can_jump = true;
	}
	else
	{
		if (player.stats.curr_speed == 0 && player.stats.speed_y == 0 && !can_jump)
			can_jump = true;

		if (player.stats.curr_speed < 0) {

			if (player.stats.curr_speed + player.stats.accel / 2 >= 0)
				player.stats.curr_speed = 0;
			else
				player.stats.curr_speed += player.stats.accel / 2;
		}
		else if (player.stats.curr_speed > 0) {
			if (player.stats.curr_speed - player.stats.accel / 2 <= 0)
				player.stats.curr_speed = 0;
			else
				player.stats.curr_speed -= player.stats.accel / 2;
		}
	}
	if ((player.state == fall) && player.stats.speed_y <= player.stats.max_speed.y + player.stats.gravity)
	{
		player.stats.speed_y += player.stats.gravity;

	}
	else {
		Jump();
	}
	// Draw everything --------------------------------------

}

void j1Player::Jump() {
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && can_jump == true) {
		if (!is_jumping)
			player.stats.speed_y = player.stats.jump_force;

		is_jumping = true;
		if (player.stats.speed_y < -6)
			player.stats.speed_y /= 1.07;

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

	else player.stats.speed_y += player.stats.gravity;
}


void j1Player::OnCollision(Collider* source, Collider* other, SDL_Rect& res_rect)
{
	//What type of collision and do
	if (source->type == COLLIDER_HOOK_RANGE) {
		if (other->type == COLLIDER_HOOK_RING) {

		}
	}

	else if (source->type == COLLIDER_PLAYER_AIR) {
		if (other->type == COLLIDER_GROUND) {
			if (abs(res_rect.w) > abs(res_rect.h)) {
				player.stats.speed_y = 0;
			}
		}
	}

	else if (source->type == COLLIDER_PLAYER_WALL) {
		if (other->type == COLLIDER_GROUND) {
			if (abs(res_rect.w) < abs(res_rect.h)) {
				// AQUI COLLISION AMB WALLS, SIMILAR AL QUE HI HA A PLAYER PERO UNA MICA DIFERENT PERQUE VAGI
			}
		}
	}

	else if (source->type == COLLIDER_PLAYER) {

		if (other->type == COLLIDER_GROUND)
		{
			// Correct position
		
			if (abs(res_rect.h) > abs(res_rect.w) && player.state != fall && player.state != jump) {
				if (player.state != fall && player.state != jump) {
					if (source->rect.x < other->rect.x) {
						player.position.x = other->rect.x - player.collision_box->rect.w - 1;
						source->rect.x = player.position.x;
						player.air_box->rect.x = player.position.x;
					}
					else if (source->rect.x > other->rect.x) {
						player.position.x = other->rect.x + other->rect.w + 1;
						source->rect.x = player.position.x;
						player.air_box->rect.x = player.position.x;
					}
					if (abs(player.stats.curr_speed) > 0) {
						player.stats.curr_speed = 0;
						if (player.stats.speed_y > 0)
							player.stats.speed_y /= 2;
						can_jump = true;
						player.stats.curr_speed *= -1;
					}
				}
				else if (abs(res_rect.h) < abs(res_rect.w)) {
					if (SDL_IntersectRect(&source->rect, &other->rect, &res_rect) > 0) {
						if (source->rect.x < other->rect.x) {
							player.position.x = other->rect.x - player.collision_box->rect.w - 1;
							source->rect.x = player.position.x;
							player.air_box->rect.x = player.position.x;
						}
						else if (source->rect.x > other->rect.x) {
							player.position.x = other->rect.x + other->rect.w + 1;
							source->rect.x = player.position.x;
							player.air_box->rect.x = player.position.x;
						}
						if (abs(player.stats.curr_speed) > 0) {
							if (player.stats.speed_y > 0)
								player.stats.speed_y /= 2;
							can_jump = true;
							player.stats.curr_speed *= -1 / 2;
						}
					}
				}
				if (abs(res_rect.h) > abs(res_rect.w) && (player.state == fall || player.state == jump)) {
					if (player.state != fall && player.state != jump) {
						if (source->rect.x < other->rect.x) {
							player.position.x = other->rect.x - player.collision_box->rect.w - 1;
							source->rect.x = player.position.x;
							player.air_box->rect.x = player.position.x;
						}
						else if (source->rect.x > other->rect.x) {
							player.position.x = other->rect.x + other->rect.w + 1;
							source->rect.x = player.position.x;
							player.air_box->rect.x = player.position.x;
						}
						if (abs(player.stats.curr_speed) > 0) {
							player.stats.curr_speed = 0;
							if (player.stats.speed_y > 0)
								player.stats.speed_y /= 2;
							can_jump = true;
							player.stats.curr_speed *= -1;
						}
					}

				}

				else if (abs(res_rect.w) > abs(res_rect.h) && abs(res_rect.w) > 0) {
					player.position.y = other->rect.y - player.collision_box->rect.h;
					source->rect.y = player.position.y;
					player.air_box->rect.y = player.position.y + player.collision_box->rect.h;



				}
			}

		}
		else if (other->type == COLLIDER_END)
		{

		}
		else if (other->type == COLLIDER_DIE)
		{

		}


	}
}

bool j1Player::Load(const pugi::xml_node& savegame) {
	bool ret = true;

	App->map->EraseMap();
	App->map->Load(savegame.child("current_map").attribute("source").as_string());

	player.stats.curr_speed = savegame.child("stats").child("speed").attribute("x").as_float();
	player.stats.speed_y = savegame.child("stats").child("speed").attribute("y").as_float();

	player.position.x = savegame.child("position").attribute("x").as_float();
	player.position.y = savegame.child("position").attribute("y").as_float();
	player.state = (player_state)savegame.child("state").attribute("type").as_int();
	player.flip = savegame.child("flip").attribute("value").as_bool();

	return ret;
}

bool j1Player::Save(pugi::xml_node& savegame) {
	bool ret = true;

	savegame.append_child("current_map").append_attribute("source") = App->map->Maps->map_file.GetString();
	savegame.append_child("stats").append_child("speed").append_attribute("x") = player.stats.curr_speed;
	savegame.child("stats").child("speed").append_attribute("y") = player.stats.speed_y;
	savegame.append_child("position").append_attribute("x") = player.position.x;
	savegame.child("position").append_attribute("y") = player.position.y;
	savegame.append_child("state").append_attribute("type") = player.state;
	savegame.append_child("flip").append_attribute("value") = player.flip;

	return ret;
}


bool j1Player::LoadSprites(const pugi::xml_node& sprite_node) {
	bool ret = true;


	p2SString source = sprite_node.child("texture").attribute("value").as_string();
	player.graphics = App->tex->Load(source.GetString());
	pugi::xml_node animation = sprite_node.child("animations").child("animation");

	while (animation.attribute("name").as_string() != "") {
		Animation* anim = new Animation;

		anim->anim_state = GetState(animation.attribute("name").as_string());
		anim->loop = animation.attribute("loop").as_bool();
		anim->speed = animation.attribute("speed").as_float();
		anim->offset_x = animation.attribute("offset_x").as_int();
		anim->offset_y = animation.attribute("offset_y").as_int();
		pugi::xml_node rect = animation.child("rect");
		for (int j = 0; rect.attribute("x").as_string() != ""; j++) {
			anim->frames[j].x = rect.attribute("x").as_uint();
			anim->frames[j].y = rect.attribute("y").as_uint();
			anim->frames[j].w = sprite_node.child("animations").attribute("w").as_uint();
			anim->frames[j].h = sprite_node.child("animations").attribute("h").as_uint();
			rect = rect.next_sibling("rect");
		}
		player.animations.add(anim);
		animation = animation.next_sibling("animation");
	}


	return ret;
}

player_state j1Player::GetState(const p2SString& state_node) {

	if (state_node == "idle") return player_state::idle;
	else if (state_node == "move") return player_state::move;
	else if (state_node == "jump") return player_state::jump;
	else if (state_node == "fall") return player_state::fall;
	return player_state::error;

}

bool j1Player::LoadProperties(const pugi::xml_node& property_node) {

	bool ret = true;

	LoadSprites(property_node);

	player.render_scale = property_node.child("render_scale").attribute("value").as_float();

	player.stats.jump_force = property_node.child("jump_force").attribute("value").as_int();
	player.stats.max_speed.x = property_node.child("speed").attribute("x").as_float();
	player.stats.max_speed.y = property_node.child("speed").attribute("y").as_float();
	player.stats.accel = property_node.child("acceleration").attribute("value").as_float();
	player.stats.gravity = property_node.child("gravity").attribute("value").as_float();
	player.stats.curr_speed = 0;
	player.state = player_state::idle;
	player.current_animation = player.FindAnimByName(player.state);

	player.collision_box = App->collision->AddCollider(
	{
		(int)player.position.x + property_node.child("collision_box").attribute("offset_x").as_int(),
		(int)player.position.y + property_node.child("collision_box").attribute("offset_y").as_int()
	},
		COLLIDER_PLAYER,
		App->player);

	player.air_box = App->collision->AddCollider(
	{
		(int)player.position.x + property_node.child("collision_box").attribute("offset_x").as_int(),
		(int)player.position.y + property_node.child("collision_box").attribute("offset_y").as_int() - 1
	},
		COLLIDER_PLAYER_AIR,
		App->player);

	player.wall_box = App->collision->AddCollider(
	{
		(int)player.position.x + property_node.child("collision_box").attribute("offset_x").as_int() - 1,
		(int)player.position.y + property_node.child("collision_box").attribute("offset_y").as_int()
	},
		COLLIDER_PLAYER_WALL,
		App->player);


	return ret;
}



