#ifndef _Timer_H_
#define _Timer_H_

#include "UI_Elements.h"
#include "Label.h"
#include "j1Timer.h"

class Timer : public UI_Element {
public:
	Timer() {}

	Timer(SDL_Rect& rect, float size) { image_rect = rect; scale = size; }

	~Timer() {}

public:
	bool Awake(const pugi::xml_node& config);
	bool Start();
	bool SpecificPreUpdate();
	bool SpecificPostUpdate();
	bool Draw();
	bool CleanUp();

public:
	int max_time;
	Label content;
	j1Timer timer;
};

bool Timer::Awake(const pugi::xml_node& config)
{
	max_time = config.attribute("max_time").as_int();
	content.content.create("%i", max_time);
	position = { config.attribute("posx").as_int(), config.attribute("posy").as_int() };

	content.scale = scale;

	group = (ui_set)config.attribute("group").as_int();

	return true;
}

bool Timer::Start()
{
	position.x = -App->render->camera.x + image_rect.x;
	position.y = -App->render->camera.y + image_rect.y;

	timer.Start();

	return true;
}

bool Timer::SpecificPreUpdate()
{
	content.content.Clear();
	content.content.create("%i", max_time - (int)timer.ReadSec() / 1);

	position.x = -App->render->camera.x + image_rect.x;
	position.y = -App->render->camera.y + image_rect.y;

	return true;
}

bool Timer::SpecificPostUpdate()
{
	Draw();

	if (max_time - (int)timer.ReadSec() / 1 <= 0 && App->entities->GetPlayer() != nullptr) {
		App->entities->GetPlayer()->stats.hp = 0;
		timer.Start();
	}

	if (App->entities->GetPlayer() != nullptr) {
		if (App->entities->GetPlayer() <= 0)
			timer.Start();
	}

	return true;
}

bool Timer::Draw()
{
	content.Draw(position);

	return true;
}

bool Timer::CleanUp()
{
	content.CleanUp();

	return true;
}

#endif //_Timer_H_