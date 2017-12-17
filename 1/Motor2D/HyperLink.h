#ifndef _HyperLink_H_
#define _HyperLink_H_


#include "Interactables.h"

class HyperLink : public Interactable {
public:
	HyperLink() {}
	HyperLink(SDL_Rect& rect, float size) { coll_rect = image_rect = rect; scale = size; }

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool SpecificPreUpdate();
	bool SpecificPostUpdate();
	bool Draw();
	bool CleanUp();

	void OnClick();
	void OnHover();

public:

};

bool HyperLink::Awake(pugi::xml_node& config)
{
	group = (ui_set)config.attribute("group").as_int();

	return true;
}

bool HyperLink::Start()
{
	return true;
}

bool HyperLink::SpecificPreUpdate()
{
	return true;
}

bool HyperLink::SpecificPostUpdate()
{
	return true;
}

bool HyperLink::Draw()
{
	return true;
}

bool HyperLink::CleanUp()
{
	return true;
}

void HyperLink::OnClick()
{

}

void HyperLink::OnHover()
{

}

#endif //_HyperLink_H_