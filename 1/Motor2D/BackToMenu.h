#ifndef _BackToMenu_H_
#define _BackToMenu_H_

#include "Button.h"
#include "j1Gui.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "EntityManager.h"
#include "j1Pathfinding.h"

class BackToMenu : public Button
{
public:
	BackToMenu(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

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

#endif //_BackToMenu_H_