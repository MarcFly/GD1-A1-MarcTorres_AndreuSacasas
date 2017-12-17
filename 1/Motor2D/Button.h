#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Interactables.h"

enum button_type
{

};

class Button : public Interactable {
public:
	Button() {}

	Button(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool SpecificPreUpdate();
	bool SpecificPostUpdate();
	bool Draw();
	bool CleanUp();

public:
	Label text;
	iPoint text_offset;
};

bool Button::Awake(pugi::xml_node& config)
{
	text.content.create(config.attribute("label").as_string());
	text.font = App->font->default;

	position = { config.attribute("posx").as_int(), config.attribute("posy").as_int() };
	
	text_offset = { config.attribute("offx").as_int(), config.attribute("offy").as_int() };

	text.scale = scale;

	group = (ui_set)config.attribute("group").as_int();

	return true;
}

bool Button::Start()
{
	return true;
}

bool Button::SpecificPreUpdate()
{
	return true;
}

bool Button::SpecificPostUpdate()
{
	Draw();
	return true;
}

bool Button::Draw()
{
	App->render->Blit(point_atlas, position.x, position.y, &image_rect);
	text.Draw({ position.x + text_offset.x, position.y + text_offset.y });
	
	return true;
}

bool Button::CleanUp()
{
	text.CleanUp();

	return true;
}

#endif //_BUTTON_H_