#ifndef _Counter_H_
#define _Counter_H_

#include "UI_Elements.h"
#include "Label.h"

class Counter : public UI_Element {
public:
	Counter() {}

	Counter(SDL_Rect& rect, float size) { image_rect = rect; scale = size; }

	bool Start();
	bool SpecificPostUpdate();
	bool Draw();
	bool CleanUp();

public:
	int* counter = nullptr;
	Label content;
};

bool Counter::Start()
{
	bool ret = true;

	content.scale = scale;

	return ret;
}

bool Counter::SpecificPostUpdate()
{
	bool ret = true;

	position = { -App->render->camera.x + image_rect.x, -App->render->camera.y + image_rect.y };

	if (counter != nullptr)
	{
		content.content.Clear();
		content.content.create("%i", *counter);

		ret = Draw();
	}

	counter = nullptr;

	return ret;
}

bool Counter::Draw()
{
	bool ret = true;

	content.Draw(position);

	return ret;
}

bool Counter::CleanUp()
{
	bool ret = true;

	counter = nullptr;

	ret = content.CleanUp();

	return ret;
}

#endif //_Counter_H_