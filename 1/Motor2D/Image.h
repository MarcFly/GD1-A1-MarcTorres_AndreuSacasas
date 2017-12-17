#ifndef _Image_H_
#define _Image_H_

#include "UI_Elements.h"


class Image : public UI_Element {
public:
	Image() {}

	Image(SDL_Rect& rect, float size) { image_rect = rect; scale = size; }

	bool Awake(const pugi::xml_node& config);
	bool Start();
	bool SpecificPreUpdate();
	bool SpecificPostUpdate();
	bool Draw();
	bool CleanUp();

public:
	SDL_Texture* external_img = nullptr;
};

bool Image::Awake(const pugi::xml_node& config)
{
	group = (ui_set)config.attribute("group").as_int();

	return true;
}

bool Image::Start()
{
	position.x -= App->render->camera.x;
	position.y -= App->render->camera.y;

	return true;
}

bool Image::SpecificPreUpdate()
{
	return true;
}

bool Image::SpecificPostUpdate()
{
	Draw();
	return true;
}

bool Image::Draw()
{
	App->render->Blit(point_atlas, position.x, position.y, &image_rect);

	return true;
}

bool Image::CleanUp()
{
	return true;
}

#endif //_Image_H_