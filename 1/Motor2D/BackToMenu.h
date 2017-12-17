#ifndef _Continue_H_
#define _Continue_H_

#include "Button.h"
#include "j1Gui.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "EntityManager.h"
#include "j1Pathfinding.h"

class Continue : public Button
{
	void OnClick()
	{
		if ((ui_set)App->gui->Get_ActiveSet() == ingame)
		{
			App->map->EraseMap();
			App->entities->CleanEntities();
			App->pathfinding->ResetNav();
			App->collisions->CleanColliders();
		}

		App->gui->Set_ActiveSet((int)menu);
	}
};

#endif //_Continue_H_