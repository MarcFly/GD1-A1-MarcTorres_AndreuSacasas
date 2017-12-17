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

class Interactable : public UI_Element
{
public:

	virtual bool SpecificPreUpdate() {return true;}

	bool PreUpdate();

	virtual bool SpecificPostUpdate() { return true; }

	bool PostUpdate();

public:

	mouse_state CheckHover();

	bool CheckClick();

public:
	virtual void OnClick() {};
	virtual void OnHover() {};
	
	bool is_click = false;
	SDL_Rect coll_rect;

	mouse_state state;
};

#endif //_INTERACTABLES_H_