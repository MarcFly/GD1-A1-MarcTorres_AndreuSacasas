#include "Entity.h"

void Entity::Draw(float dt) {

	current_animation = FindAnimByName(state);

	switch (state)
	{
	case move_state::idle:
		current_anim_size = 7;
		break;
	case move_state::move:
		current_anim_size = 9;
		break;
	case move_state::jump:
		current_anim_size = 5;
		break;
	case move_state::fall:
		current_anim_size = 3;
		break;
	default:
		break;
	}


	App->render->Blit(graphics, position.x - current_animation->offset_x, position.y - current_animation->offset_y, &current_animation->frames[current_animation->GetAnimationFrame(dt, current_anim_size)], render_scale, flip);
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

	p2SString source = sprite_node.child("texture").attribute("value").as_string();
	graphics = App->tex->Load(source.GetString());
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
		animations.add(anim);
		animation = animation.next_sibling("animation");
	}

	return ret;
}