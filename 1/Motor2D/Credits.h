#ifndef _Credits_H_
#define _Credits_H_

#include "Button.h"
#include "j1Gui.h"

class Credits : public Button
{
	void OnClick()
	{
		App->gui->Set_ActiveSet((int)credits);
	}
};

#endif //_Credits_H_