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

#include<stdio.h>
#include<math.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");
	
}

j1Player::~j1Player()
{}

// Awake, actually load things?
bool j1Player::Awake(const pugi::xml_node& config)
{
	bool ret = true;
	 
	local_node = config;
	
	return ret;
}
// Load assets
bool j1Player::Start()
{
	bool ret = true;

	pugi::xml_document* doc = new pugi::xml_document;
	pugi::xml_parse_result result = doc->load("sprites.xml");

	if (result == NULL) { //Check that it loaded
		LOG("Could not load sprite xml file player_sprites.xml. pugi error: %s", result.description());
		ret = false;
	}
	else {
		LoadProperties(doc->child("sprites").child("player"));
	}

	delete doc;
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

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		player.flip = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		player.flip = true;
	}

		
	
	//Move player
		

	// Draw everything --------------------------------------

	//App->render->Blit(main_player.graphics, main_player.position.x - main_player.offset.x, main_player.position.y - main_player.offset.y, &main_player.current_animation->GetCurrentFrame(), main_player.render_scale);

return true;

}

void j1Player::Jump() {

}


void j1Player::Movement() {

}

void j1Player::Hook() {

}


void j1Player::OnCollision()
{
	//What type of collision and do
}

bool j1Player::Load(const pugi::xml_node& savegame) {
	bool ret = true;

	return ret;
}

bool j1Player::Save(const pugi::xml_node& savegame) {
	bool ret = true;

	return ret;
}


bool j1Player::LoadSprites(const pugi::xml_node& sprite_node) {
	bool ret = true;

	
	p2SString source = sprite_node.child("texture").attribute("value").as_string();
	player.graphics = App->tex->Load(source.GetString());
	pugi::xml_node animation = sprite_node.child("animations").child("animation");

	while (animation.attribute("name").as_string() != "") {
		Animation* anim = new Animation;
			
		anim->anim_state = Get_State(animation.attribute("name").as_string());
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

player_state j1Player::Get_State(const p2SString& state_node) {

	if (strcmp(state_node.GetString(), "idle")) return idle;
	if (strcmp(state_node.GetString(), "moving")) return move;
	if (strcmp(state_node.GetString(), "running")) return running;
	if (strcmp(state_node.GetString(), "air")) return air;
	if (strcmp(state_node.GetString(), "jumpsquat")) return jumpsquat;
	if (strcmp(state_node.GetString(), "landing")) return landing;
	if (strcmp(state_node.GetString(), "hooking")) return hooking;
	if (strcmp(state_node.GetString(), "hooked")) return hooked;
	return error;

}

bool j1Player::LoadProperties(const pugi::xml_node& property_node) {

	bool ret = true;

	LoadSprites(property_node);

	player.position.x = 0;//READ FROM MAP
	player.position.y = 0;//READ FROM MAP

	player.render_scale = property_node.child("render_scale").attribute("value").as_float();

	player.stats.jump_force = property_node.child("jump_force").attribute("value").as_int();
	player.stats.max_speed = property_node.child("speed").attribute("value").as_float();
	player.stats.accel = property_node.child("accel").attribute("value").as_float();
	player.stats.gravity = property_node.child("gravity").attribute("value").as_float();
	player.stats.hook_range = property_node.child("hook_range").attribute("value").as_float();
	player.stats.aerial_drift = property_node.child("aerial_drift").attribute("value").as_float();
	player.stats.curr_speed = 0;

	player.collision_box->rect = 
	{ 
		player.position.x + property_node.child("collision_box").attribute("offset_x").as_int(), 
		player.position.y + property_node.child("collision_box").attribute("offset_y").as_int(),
		property_node.child("collision_box").attribute("w").as_int(), 
		property_node.child("collision_box").attribute("h").as_int() 
	};
	
	return ret;
}

