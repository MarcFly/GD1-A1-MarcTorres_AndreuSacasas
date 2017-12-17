#ifndef _VOLUMEUP_H_
#define _VOLUMEUP_H_

#include "Button.h"
#include "j1Audio.h"
#include "j1Gui.h"

class VolumeUp : public Button
{
public:
	VolumeUp(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

	void OnClick()
	{
		App->audio->Increase_Master();
	}

};

#endif //_VOLUMEUP_H_