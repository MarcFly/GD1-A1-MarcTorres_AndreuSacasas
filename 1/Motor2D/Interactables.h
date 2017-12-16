#ifndef _INTERACTABLES_H_
#define _INTERACTABLES_H_

#include "UI_Elements.h"

enum mouse_state {
	Enter = 0,
	Stay,
	Leave,
	Out,

	mouse_max
};

class Interactable : public UI_Element {
public:

	virtual bool SpecificPreUpdate() {return true;}

	bool PreUpdate()
	{
		bool ret = true;

		state = CheckHover();

		is_click = CheckClick();

		ret = SpecificPreUpdate();

		return ret;
	}

	virtual bool SpecificPostUpdate() { return true; }

	bool PostUpdate()
	{
		bool ret = true;

		if (is_click && state == Stay)
			OnClick();

		if (!is_click && (state == Enter || state == Stay))
			OnHover();

		ret = SpecificPostUpdate();

		return ret;
	}

public:
	mouse_state CheckHover()
	{		
		SDL_Rect result;
		SDL_Rect mouse = { 0,0,2,2 };
		App->input->GetMousePosition(mouse.x, mouse.y);

		SDL_IntersectRect( &mouse, &coll_rect, &result);

		if ((result.w > 0 && result.h > 0) && (state == Leave || state == Out))
			return Enter;

		if ((result.w > 0 && result.h > 0) && (state == Enter || state == Stay))
			return Stay;
		
		if ((result.w < 0 || result.h < 0) && (state == Enter || state == Stay))
			return Leave;
		
		return Out;
	};

	bool CheckClick()
	{
		bool ret = false;

		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) {
			if(state == Stay)
				OnClick();

			return ret;
		}

		return ret;
	}

public:
	virtual void OnClick() {};
	virtual void OnHover() {};
	
	bool is_click = false;
	SDL_Rect coll_rect;

	mouse_state state;
};

#endif //_INTERACTABLES_H_