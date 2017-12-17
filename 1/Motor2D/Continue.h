#ifndef _Continue_H_
#define _Continue_H_

#include "Button.h"
#include "j1Gui.h"

class Continue : public Button
{
public:
	Continue(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

	void OnClick()
	{
		App->Trigger_Load();
		App->gui->Set_ActiveSet((int)ingame);
	}
};

#endif //_Continue_H_