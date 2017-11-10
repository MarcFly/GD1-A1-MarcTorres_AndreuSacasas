#include "Entity.h"
#include "EntityManager.h"
#include "j1App.h"
#include "j1Map.h"

void Entity::Draw(float dt) {
	if (this != nullptr) {
		current_animation = FindAnimByName(state);


		App->render->Blit(*graphics,
			position.x - current_animation->offset_x,
			position.y - current_animation->offset_y,
			&current_animation->frames[current_animation->GetAnimationFrame(dt, current_animation->frames.Count())],
			current_animation->speed,
			0.0,
			render_scale,
			flip);
	}
}

void Entity::UpdateState() {

	if (stats.speed.y < 0)
		state = jump;
	else if (stats.speed.y > 0)
		state = fall;
	else if (abs(stats.speed.x) > stats.accel.x)
		state = move;
	else if (stats.speed.x == 0 && stats.speed.x == 0)
		state = idle;

	if (stats.speed.x > 0)
		flip = false;
	else if (stats.speed.x < 0)
		flip = true;
}

bool Entity::LoadSprites(const pugi::xml_node& sprite_node) {
	bool ret = true;

	p2SString source("%s%s", App->entities->tex_folder.GetString(), sprite_node.attribute("source").as_string());

	tex item(source);
	if (App->entities->texs.find(item) == -1) {

		App->entities->texs.add(item);
		graphics = &App->entities->texs.end->data.texture;
	
	}
	else {
		graphics = &App->entities->texs[App->entities->texs.find(item)].texture;
	}

	pugi::xml_node animation = sprite_node.child("animation");

	while (animation.attribute("name").as_string() != "") {
		Animation* anim = new Animation;

		anim->anim_state = GetState(animation.attribute("name").as_string());
		anim->loop = animation.attribute("loop").as_bool();
		anim->speed = animation.attribute("speed").as_float();
		anim->offset_x = animation.attribute("offset_x").as_int();
		anim->offset_y = animation.attribute("offset_y").as_int();
		pugi::xml_node rect = animation.child("rect");
		for (int j = 0; rect.attribute("x").as_string() != ""; j++) {
			anim->frames.PushBack({
				rect.attribute("x").as_int(),
				rect.attribute("y").as_int(),
				sprite_node.attribute("w").as_int(),
				sprite_node.attribute("h").as_int() });

			rect = rect.next_sibling("rect");
		}
		animations.add(anim);
		animation = animation.next_sibling("animation");
	}

	return ret;
}

bool Entity::LoadProperties(const pugi::xml_node& property_node) {
	bool ret = true;

	position = {property_node.child("position").attribute("x").as_int(), property_node.child("position").attribute("y").as_int()};
	stats.max_speed = { property_node.child("max_speed").attribute("x").as_int(), property_node.child("max_speed").attribute("x").as_int() };
	stats.accel = { property_node.child("acceleration").attribute("x").as_float(),property_node.child("acceleration").attribute("y").as_float() };
	stats.jump_force = property_node.child("jump_force").attribute("value").as_int();
	stats.speed = { property_node.child("speed").attribute("x").as_float(), property_node.child("speed").attribute("y").as_float() };
	flip = property_node.child("flip").attribute("value").as_bool();
	state = (move_state)property_node.child("state").attribute("value").as_int();
	render_scale = property_node.child("render_scale").attribute("value").as_float();

	if(type == 0)
		collision_box = App->collisions->AddCollider({
				position.x,
				position.y,
				property_node.child("collision_box").attribute("w").as_int(),
				property_node.child("collision_box").attribute("h").as_int()},
			COLLIDER_PLAYER,
			App->entities);

	else
		collision_box = App->collisions->AddCollider({
				position.x,
				position.y,
				property_node.child("collision_box").attribute("w").as_int(),
				property_node.child("collision_box").attribute("h").as_int() },
			COLLIDER_ENEMY,
			App->entities);

	return ret;
}

bool Entity::Load(const pugi::xml_node& savegame)
{
	bool ret = true;

	LoadProperties(savegame);

	pugi::xml_node temp_sprite = App->entities->sprites_doc.child("sprites").child("animations");

	while (temp_sprite.attribute("type").as_int() != type)
		temp_sprite = temp_sprite.next_sibling();

	LoadSprites(temp_sprite);

	return ret;
}

bool Entity::Save(pugi::xml_node& savegame)
{
	bool ret = true;

	savegame.append_child("entity").append_attribute("type") = type;
	savegame.child("entity").append_attribute("entity_id") = entity_id;

	savegame.child("entity").append_child("speed").append_attribute("x") = stats.speed.x;
	savegame.child("entity").child("speed").append_attribute("y") = stats.speed.y;

	savegame.child("entity").append_child("position").append_attribute("x") = position.x;
	savegame.child("entity").child("position").append_attribute("y") = position.y;

	savegame.child("entity").append_child("state").append_attribute("type") = state;
	savegame.child("entity").append_child("flip").append_attribute("value") = flip;

	savegame.child("entity").append_child("collision_box").append_attribute("offset_x") = "";
	savegame.child("entity").child("collision_box").append_attribute("offset_y") = "";
	savegame.child("entity").child("collision_box").append_attribute("w") = collision_box->rect.w;
	savegame.child("entity").child("collision_box").append_attribute("h") = collision_box->rect.h;

	savegame.child("entity").append_child("acceleration").append_attribute("x") = stats.accel.x;
	savegame.child("entity").child("acceleration").append_attribute("y") = stats.accel.y;

	savegame.child("entity").append_child("jump_force").append_attribute("value") = stats.jump_force;

	savegame.child("entity").append_child("max_speed").append_attribute("x") = stats.max_speed.x;
	savegame.child("entity").child("max_speed").append_attribute("y") = stats.max_speed.y;

	savegame.child("entity").append_child("render_scale").append_attribute("value") = render_scale;

	return ret;
}