#ifndef _Settings_H_
#define _Settings_H_

#include "Button.h"
#include "j1Gui.h"

class Settings : public Button
{
public:
	Settings(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

	void OnClick()
	{
		App->gui->Set_ActiveSet((int)settings);
	}
};

#endif //_Settings_H_