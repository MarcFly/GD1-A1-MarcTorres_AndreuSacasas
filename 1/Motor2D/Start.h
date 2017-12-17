#ifndef _Start_H_
#define _Start_H_

#include "Button.h"
#include "j1Scene.h"
#include "j1Gui.h"

class Start : public Button
{
	void OnClick()
	{
		App->scene->curr_map = 0;
		App->scene->LoadMap(App->scene->curr_map);
		App->gui->Set_ActiveSet((int)ingame);
	}

};

#endif //_Start_H_