#ifndef _Start_b_H_
#define _Start_b_H_

#include "Button.h"
#include "j1Scene.h"
#include "j1Gui.h"

class Start_b : public Button
{
public:
	Start_b(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

	void OnClick()
	{
		App->scene->curr_map = 0;
		App->scene->LoadMap(App->scene->curr_map);
		App->gui->Set_ActiveSet((int)ingame);
	}

};

#endif //_Start_b_H_