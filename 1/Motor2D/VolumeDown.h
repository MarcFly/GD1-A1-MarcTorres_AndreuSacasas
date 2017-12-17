#ifndef _VOLUMEDOWN_H_
#define _VOLUMEDOWN_H_

#include "Button.h"
#include "j1Audio.h"
#include "j1Gui.h"

class VolumeDown : public Button
{
public:
	VolumeDown(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

	void OnClick()
	{
		App->audio->Decrease_Master();
	}

};

#endif //_VOLUMEDOWN_H_