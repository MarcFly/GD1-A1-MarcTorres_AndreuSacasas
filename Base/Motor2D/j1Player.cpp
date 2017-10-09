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

#include<stdio.h>
#include<math.h>

j1Player::j1Player()
{
	name.create("player");
	
}

j1Player::~j1Player()
{}

// Awake, actually load things?
bool j1Player::Awake(const pugi::xml_node& config)
{
	bool ret = true;

	Local_config = config;
	 
	
	return ret;
}
// Load assets
bool j1Player::Start()
{
	bool ret = true;

	// Inicializar lo necesario del jugador, crear los personajes en el mapa

	ret = LoadProperties(Local_config.child("properties"));



	pugi::xml_node node = sprites.child("sprites").child("player");
	ret = LoadSprites(node);
	
	main_player.state = idle;
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

	if (!main_player.jumping)
	{ 
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			
			Jump(character_controll);
			main_player.jumping = true;
			
			
		}
	}



		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
			if (!character_controll)
			SlideStart();
			else
			{
				if(!main_player.jumping)
				{ 
				   if(!characters[1].anchored)
				   AnchorStart();
				   else
				   AnchorEnd();
				   }
			}
			

			//Gordete anchor
		}

		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_UP)
		{
			if (!character_controll)
				SlideEnd();
			else
			{
				
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			GoRight(character_controll);
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			GoLeft(character_controll);
		}
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			character_controll = !character_controll;
		}

	

	//Move player
		if (!main_player.moving)
		{
			StopMoving(false);
	    }

		if (!characters[1].moving)
		{
			StopMoving(true);
		}

		main_player.player->TateQuieto();

		characters[1].player->TateQuieto();
	// Draw everything --------------------------------------


		if (main_player.sliding)
		{
			int x, y;
			main_player.player_sliding->GetPosition(x, y);
			main_player.real_position.x = x;
			main_player.real_position.y = y;
		}
		else
		{
			int x, y;
			main_player.player->GetPosition(x, y);
			main_player.real_position.x = x;
			main_player.real_position.y = y;
		}

		

		if (characters[1].anchored)
		{
			int x, y;
			characters[1].player_anchor->GetPosition(x, y);
			characters[1].real_position.x = x;
			characters[1].real_position.y = y;
		}
		else
		{
			int x, y;
			characters[1].player->GetPosition(x, y);
			characters[1].real_position.x = x;
			characters[1].real_position.y = y;
		}

		App->render->Blit(main_player.graphics, main_player.position.x - main_player.offset.x, main_player.position.y - main_player.offset.y, &main_player.current_animation->GetCurrentFrame(), main_player.render_scale);

return true;

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

	
	p2SString source = sprite_node.attribute("source").as_string();
	main_player.graphics = App->tex->Load(source.GetString());
	pugi::xml_node animation = sprite_node.child("animation");

	while (animation.attribute("name").as_string() != "") {
		Animation* anim = new Animation;
			
		anim->anim_state = Get_State(animation.attribute("name").as_string());
		anim->loop = animation.attribute("loop").as_bool();
		anim->speed = animation.attribute("speed").as_float();
			
		pugi::xml_node image = animation.child("image");
		for (int j = 0; image.attribute("w").as_int() != 0; j++) {
			anim->frames[j].x = image.attribute("x").as_uint();
			anim->frames[j].y = image.attribute("y").as_uint();
			anim->frames[j].w = image.attribute("w").as_uint();
			anim->frames[j].h = image.attribute("h").as_uint();
			image = image.next_sibling("image");
		}
		main_player.animations.add(anim);
		animation = animation.next_sibling("animation");
	}
		

	return ret;
}

player_state j1Player::Get_State(const p2SString& state_node) {

	if (strcmp(state_node.GetString(), "idle")) return idle;
	if (strcmp(state_node.GetString(), "moving")) return moving;
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

	pugi::xml_parse_result result = sprites.load(property_node.attribute("source").as_string());
	
	LoadSprites(sprites.child("main_player"));
	
	main_player.offset.x = property_node.child("offset").attribute("x").as_uint();
	main_player.offset.y = property_node.child("offset").attribute("y").as_uint();
	main_player.position.x = property_node.child("position").attribute("x").as_int();//READ HERE FROM XML
	main_player.position.y = property_node.child("position").attribute("y").as_int();//READ HERE FROM XML

	main_player.render_scale = property_node.child("render_scale").attribute("value").as_float();

	main_player.stats.jump_force = property_node.child("jump_force").attribute("value").as_int();
	main_player.stats.max_speed = property_node.child("max_speed").attribute("value").as_float(); //cambiar con xml
	main_player.stats.accel = property_node.child("accel").attribute("value").as_float();
	main_player.stats.gravity = property_node.child("gravity").attribute("value").as_float();
	main_player.stats.hook_range = property_node.child("hook_range").attribute("value").as_float();
	main_player.stats.aerial_drift = property_node.child("aerial_drift").attribute("value").as_float();
		
	main_player.name.create(property_node.attribute("name").as_string());

	return ret;
}

