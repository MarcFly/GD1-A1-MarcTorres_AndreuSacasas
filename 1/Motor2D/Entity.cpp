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
	else if (stats.speed.y > stats.accel.y * 2)
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

	// While no UI module

	pugi::xml_node health = sprite_node.parent().child("ui").child("health");
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

	test.create(health.attribute("source").as_string());
	

	return ret;
}

bool Entity::LoadProperties(const pugi::xml_node& property_node) {
	bool ret = true;

	position = {property_node.child("position").attribute("x").as_int(), property_node.child("position").attribute("y").as_int()};
	stats.max_speed = { property_node.child("max_speed").attribute("x").as_int(), property_node.child("max_speed").attribute("y").as_int() };
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

	else if(type == 1)
		collision_box = App->collisions->AddCollider({
				position.x,
				position.y,
				property_node.child("collision_box").attribute("w").as_int(),
				property_node.child("collision_box").attribute("h").as_int() },
			COLLIDER_CRAWLER,
			App->entities);
	else if (type == 3)
		collision_box = App->collisions->AddCollider({
		position.x,
		position.y,
		property_node.child("collision_box").attribute("w").as_int(),
		property_node.child("collision_box").attribute("h").as_int() },
		COLLIDER_FLYER,
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

	
	pugi::xml_node temp_node = savegame.append_child("entity");

	temp_node.append_attribute("type") = type;
	temp_node.append_attribute("entity_id") = entity_id;

	temp_node.append_child("speed").append_attribute("x") = stats.speed.x;
	temp_node.child("speed").append_attribute("y") = stats.speed.y;

	temp_node.append_child("position").append_attribute("x") = position.x;
	temp_node.child("position").append_attribute("y") = position.y;

	temp_node.append_child("state").append_attribute("type") = state;
	temp_node.append_child("flip").append_attribute("value") = flip;

	temp_node.append_child("collision_box").append_attribute("offset_x") = "";
	temp_node.child("collision_box").append_attribute("offset_y") = "";
	temp_node.child("collision_box").append_attribute("w") = collision_box->rect.w;
	temp_node.child("collision_box").append_attribute("h") = collision_box->rect.h;

	temp_node.append_child("acceleration").append_attribute("x") = stats.accel.x;
	temp_node.child("acceleration").append_attribute("y") = stats.accel.y;

	temp_node.append_child("jump_force").append_attribute("value") = stats.jump_force;

	temp_node.append_child("max_speed").append_attribute("x") = stats.max_speed.x;
	temp_node.child("max_speed").append_attribute("y") = stats.max_speed.y;

	temp_node.append_child("render_scale").append_attribute("value") = render_scale;

	return ret;
}