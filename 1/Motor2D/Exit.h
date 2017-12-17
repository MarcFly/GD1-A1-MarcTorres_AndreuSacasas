#ifndef _Exit_H_
#define _Exit_H_

#include "Button.h"
#include "j1Gui.h"

class Exit : public Button
{
	void OnClick()
	{
		App->gui->not_end = false;
	}
};

#endif //_Exit_H_