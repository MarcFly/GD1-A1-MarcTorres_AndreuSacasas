#include "Entity.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "EntityManager.h"

bool Entity::Start() {
	
	collision_box = App->collisions->AddCollider(*coll_rect, (COLLIDER_TYPE)(type + COLLIDER_PLAYER), App->entities);
	delete coll_rect;
	coll_rect = nullptr;

	return true;
	
}

void Entity::CopyFromTE(Entity* template_ent)
{
	graphics = template_ent->graphics;
	current_animation = template_ent->current_animation;
	
	p2List_item<Animation*>* item = template_ent->animations.start;
	while (item != NULL)
	{
		animations.add(item->data);
		item = item->next;
	}

	coll_rect = new SDL_Rect(*template_ent->coll_rect);

	stats = template_ent->stats;
	render_scale = template_ent->render_scale;
	position = template_ent->position;
	flip = template_ent->flip;
	type = template_ent->type;
	entity_id = template_ent->entity_id;

	CopySpecifics(template_ent);

}

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

	

	pugi::xml_node r_node = sprite_node.parent();
	ret = LoadSpecificSprites(r_node);
	
	

	return ret;
}

bool Entity::LoadProperties(const pugi::xml_node& property_node) {
	bool ret = true;

	position = FARLANDS;
	stats.max_speed = { property_node.child("max_speed").attribute("x").as_int(), property_node.child("max_speed").attribute("y").as_int() };
	stats.accel = { property_node.child("acceleration").attribute("x").as_float(),property_node.child("acceleration").attribute("y").as_float() };
	stats.jump_force = property_node.child("jump_force").attribute("value").as_int();
	stats.speed = { property_node.child("speed").attribute("x").as_float(), property_node.child("speed").attribute("y").as_float() };
	flip = property_node.child("flip").attribute("value").as_bool();
	state = (move_state)property_node.child("state").attribute("value").as_int();
	render_scale = property_node.child("render_scale").attribute("value").as_float();

	coll_rect = new SDL_Rect({
		position.x,
		position.y,
		property_node.child("collision_box").attribute("w").as_int(),
		property_node.child("collision_box").attribute("h").as_int()
	});

	return ret;
}

iPoint Entity::GetMapTile()
{
	iPoint ret[4] = {
		App->map->WorldToMap(position.x,position.y),
		App->map->WorldToMap(position.x + collision_box->rect.w, position.y),
		App->map->WorldToMap(position.x, position.y + collision_box->rect.h),
		App->map->WorldToMap(position.x + collision_box->rect.w, position.y + collision_box->rect.h)
	};

	iPoint in_map[4] = {
		App->map->MapToWorld(ret[0].x, ret[0].y),
		App->map->MapToWorld(ret[1].x, ret[1].y),
		App->map->MapToWorld(ret[2].x, ret[2].y),
		App->map->MapToWorld(ret[3].x, ret[3].y)
	};

	SDL_Rect rects[4] =
	{
		{ in_map[0].x, in_map[0].y, App->map->Maps->tilewidth, App->map->Maps->tileheight },
		{ in_map[1].x, in_map[1].y, App->map->Maps->tilewidth, App->map->Maps->tileheight },
		{ in_map[2].x, in_map[2].y, App->map->Maps->tilewidth, App->map->Maps->tileheight },
		{ in_map[3].x, in_map[3].y, App->map->Maps->tilewidth, App->map->Maps->tileheight }
	};

	SDL_Rect res_rect[4];

	int test[4][3] {
		{ in_map[0].x, in_map[0].y, collision_box->CheckCollision(rects[0], res_rect[0]) },
		{ in_map[1].x, in_map[1].y, collision_box->CheckCollision(rects[1], res_rect[1]) },
		{ in_map[2].x, in_map[2].y, collision_box->CheckCollision(rects[2], res_rect[2]) },
		{ in_map[3].x, in_map[3].y, collision_box->CheckCollision(rects[3], res_rect[3]) }
	};

	iPoint final_ret;
	int pre_high = 0;

	for (int i = 0; i < 4; i++) {
		if (final_ret.x == test[i][0] && final_ret.y == test[i][1] && res_rect[i].w > 0 && res_rect[i].h > 0)
			pre_high += test[i][2];

		if (test[i][2] > pre_high && res_rect[i].w > 0 && res_rect[i].h > 0) {
			final_ret = { test[i][0],test[i][1] };
			pre_high = test[i][2];
		}
	}

	return final_ret;
}

bool Entity::Load(const pugi::xml_node& savegame)
{
	bool ret = true;

	LoadProperties(savegame);

	position = { savegame.child("position").attribute("x").as_int(), savegame.child("position").attribute("y").as_int() };

	pugi::xml_node temp_sprite = App->entities->sprites_doc.child("sprites").child("animations");

	while (temp_sprite.attribute("type").as_int() != type)
		temp_sprite = temp_sprite.next_sibling();

	LoadSprites(temp_sprite);

	this->Start();

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

	if (type == 0)
		temp_node.append_child("life").append_attribute("value") = App->entities->GetPlayer()->stats.hp;

	return ret;
}