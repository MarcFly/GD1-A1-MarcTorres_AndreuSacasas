#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "Interactables.h"

class Slider : public Interactable {
public:
	Slider() {}

	Slider(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

	bool Awake(const pugi::xml_node& config);
	bool Start();
	bool SpecificPreUpdate();
	bool SpecificPostUpdate();
	bool Draw();
	bool CleanUp();

public:
	int range[2];
};

bool Slider::Awake(const pugi::xml_node& config)
{

	position = { config.attribute("posx").as_int(), config.attribute("posy").as_int() };
	range[0] = config.attribute("start").as_int();
	range[1] = config.attribute("end").as_int();

	group = (ui_set)config.attribute("group").as_int();

	return true;
}

bool Slider::Start()
{
	position.x -= App->render->camera.x;
	position.y -= App->render->camera.y;

	return true;
}

bool Slider::SpecificPreUpdate()
{
	return true;
}

bool Slider::SpecificPostUpdate()
{
	Draw();
	return true;
}

bool Slider::Draw()
{
	App->render->Blit(point_atlas, position.x, position.y, &image_rect);

	return true;
}

bool Slider::CleanUp()
{

	return true;
}

#endif //_Slider_H_