#ifndef _Settings_H_
#define _Settings_H_

#include "Button.h"
#include "j1Gui.h"

class Settings : public Button
{
	void OnClick()
	{
		App->gui->Set_ActiveSet((int)settings);
	}
};

#endif //_Settings_H_