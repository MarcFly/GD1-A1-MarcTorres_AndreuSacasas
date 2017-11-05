#include "Entity.h"
#include "EntityManager.h"
#include "j1App.h"

void Entity::Draw(float dt) {

	current_animation = FindAnimByName(state);


	App->render->Blit(*graphics, position.x - current_animation->offset_x +  App->render->camera.x, position.y - current_animation->offset_y + App->render->camera.y, &current_animation->frames[current_animation->GetAnimationFrame(dt, current_animation->frames.Count())], render_scale, flip);
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

	LOG("%s State %d", name.GetString(), state);
}

bool Entity::LoadSprites(const pugi::xml_node& sprite_node) {
	bool ret = true;

	p2SString source("%s%s", App->entities->tex_folder.GetString(), sprite_node.child("texture").attribute("source").as_string());

	tex item(source);
	if (App->entities->texs.find(item) == -1) {

		App->entities->texs.add(item);
		graphics = &App->entities->texs.end->data.texture;
	
	}
	else {
		graphics = &App->entities->texs[App->entities->texs.find(item)].texture;
	}

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
			anim->frames.PushBack({
				rect.attribute("x").as_int(),
				rect.attribute("y").as_int(),
				sprite_node.child("animations").attribute("w").as_int(),
				sprite_node.child("animations").attribute("h").as_int() });

			rect = rect.next_sibling("rect");
		}
		animations.add(anim);
		animation = animation.next_sibling("animation");
	}

	return ret;
}

bool Entity::LoadProperties(const pugi::xml_node& sprite_node) {
	bool ret = true;

	
	stats.max_speed = { sprite_node.child("max_speed").attribute("x").as_int(), sprite_node.child("max_speed").attribute("x").as_int() };
	stats.accel = { sprite_node.child("acceleration").attribute("x").as_float(),sprite_node.child("acceleration").attribute("y").as_float() };
	stats.jump_force = sprite_node.child("jump_force").attribute("value").as_int();
	stats.speed = { 0,0 };

	collision_box = App->collisions->AddCollider({
			position.x,
			position.y,
			sprite_node.child("collision_box").attribute("w").as_int(),
			sprite_node.child("collision_box").attribute("h").as_int() },
		COLLIDER_ENTITY,
		App->entities);

	return ret;
}