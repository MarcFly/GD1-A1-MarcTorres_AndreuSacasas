#ifndef _LABEL_H_
#define _LABEL_H_

#include "UI_Elements.h"


class Label : public UI_Element {
public:
	Label() {}

	Label(SDL_Rect& rect, float size) { image_rect = rect; scale = size; }

	Label(_TTF_Font* f, const char* c) { content.create(c); font = f; }

	~Label() { content.Clear(); font = nullptr; }

public:
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool SpecificPreUpdate();
	bool SpecificPostUpdate();
	bool Draw(const iPoint& pos);
	bool CleanUp();

public:
	p2SString content;
	_TTF_Font* font = nullptr;
	SDL_Texture* blit = nullptr;
};

bool Label::Awake(pugi::xml_node& config)
{
	content.create(config.attribute("content").as_string());
	font = App->font->default;
	position = { config.attribute("posx").as_int(), config.attribute("posy").as_int() };

	return true;
}

bool Label::Start()
{
	return true;
}

bool Label::SpecificPreUpdate()
{

	return true;
}

bool Label::SpecificPostUpdate()
{
	Draw(position);
	return true;
}

bool Label::Draw(const iPoint& pos)
{
	if (blit != nullptr)
		App->tex->UnLoad(blit);
	blit = nullptr;

	blit = App->font->Print(content.GetString());
	
	App->render->Blit(blit, pos.x, pos.y);

	return true;
}

bool Label::CleanUp()
{
	content.Clear();
	font = nullptr;

	if (blit != nullptr)
		App->tex->UnLoad(blit);
	blit = nullptr;

	return true;
}

#endif //_LABEL_H_