#include "Coin.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Player.h"

bool Coin::Start()
{
	bool ret = true;

	current_animation = FindAnimByName(idle);
	state = idle;

	collision_box = App->collisions->AddCollider(*coll_rect, (COLLIDER_TYPE)(type + COLLIDER_PLAYER), App->entities);
	delete coll_rect;

	return ret;
}

void Coin::CopySpecifics(Entity* template_entity)
{

}

void Coin::Draw(float dt)
{
	if (this != nullptr) {
		current_animation = FindAnimByName(state);

		collision_box->rect.x = position.x;
		collision_box->rect.y = position.y;
		
		App->render->Blit(*graphics,
			position.x - current_animation->offset_x,
			position.y - current_animation->offset_y,
			&current_animation->frames[current_animation->GetAnimationFrame(dt, current_animation->frames.Count())],
			current_animation->speed,
			0.0,
			render_scale,
			flip);
	}
	App->collisions->LookColl(this, dt);
}

void Coin::OnCollision(Collider* c1, Collider* c2, SDL_Rect& check)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		j1Player* temp = (j1Player*)App->entities->GetPlayer();
		temp->AddCoin(1);
		temp->AddScore(1 * 500);

		temp = nullptr;

		App->entities->DestroyEntity(App->entities->FindEntities(type, entity_id));
	}

}