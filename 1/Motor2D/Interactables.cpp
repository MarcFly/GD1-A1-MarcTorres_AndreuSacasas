#include "Interactables.h"
#include "j1Audio.h"
#include "j1Scene.h"

bool Interactable::PreUpdate()
{
	bool ret = true;

	state = CheckHover();

	is_click = CheckClick();

	ret = SpecificPreUpdate();

	return ret;
}

bool Interactable::PostUpdate()
{
	bool ret = true;

	if (is_click && state == Stay)
		OnClick();

	if (!is_click && (state == Enter || state == Stay))
		OnHover();

	ret = SpecificPostUpdate();

	return ret;
}

mouse_state Interactable::CheckHover()
{
	SDL_Rect result;
	SDL_Rect mouse = { 0,0,2,2 };
	App->input->GetMousePosition(mouse.x, mouse.y);

	mouse.x -= App->render->camera.x;
	mouse.y -= App->render->camera.y;

	SDL_IntersectRect(&mouse, &coll_rect, &result);

	if ((result.w > 0 && result.h > 0) && (state == Leave || state == Out))
		return Enter;

	if ((result.w > 0 && result.h > 0) && (state == Enter || state == Stay))
		return Stay;

	if ((result.w < 0 || result.h < 0) && (state == Enter || state == Stay))
		return Leave;

	return Out;
}

bool Interactable::CheckClick()
{
	bool ret = false;
	ret = (App->input->GetMouseButtonDown(1) == KEY_DOWN);
	if (ret == true) {
		if (state == Stay)
		{
			OnClick();
			App->audio->PlayFx(App->scene->click_id);
		}

		return ret;
	}

	return ret;
}
