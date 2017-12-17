
#ifndef _UI_ELEMENTS_H_
#define _UI_ELEMENTS_H_

#include "j1Input.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Fonts.h"

enum ui_set
{
	menu = 0,
	settings,
	credits,
	ingame,
	all, //for exit button that will be everywhere
	ui_set_max
};

class SDL_Rect;

class UI_Element {
public:
	virtual bool Awake(const pugi::xml_node& config) { group = (ui_set)config.attribute("group").as_int(); return true; }
	virtual bool Start() { return true; }

	virtual bool PreUpdate() { SpecificPreUpdate(); return true; }
	virtual bool SpecificPreUpdate() { return true; }

	virtual bool PostUpdate() { SpecificPostUpdate(); return true; }
	virtual bool SpecificPostUpdate() { return true; }

	virtual bool Draw() { return true; }
	virtual bool CleanUp() { return true; }


public:
	SDL_Rect image_rect;
	float scale;
	iPoint position = { 0,0 };

	ui_set group;

	SDL_Texture* point_atlas;
};

#endif //_UI_ELEMENT_H_