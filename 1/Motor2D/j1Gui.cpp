#include "p2Defs.h"
#include "p2Log.h"
#include "j1Gui.h"

#include "Label.h"
#include "Image.h"
#include "Button.h"
#include "HyperLink.h"
#include "TextBox.h"
#include "Slider.h"
#include "BackToMenu.h"
#include "Continue.h"
#include "Credits.h"
#include "Exit.h"
#include "Settings.h"
#include "Start_b.h"
#include "VolumeUp.h"
#include "VolumeDown.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(const pugi::xml_node& config)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = config.child("atlas").attribute("file").as_string("");

	pugi::xml_node object_node = config.child("objects").child("obj");

	while (object_node.attribute("type").as_string() != "")
	{
		SDL_Rect img_rect = {
			object_node.attribute("x").as_int(),
			object_node.attribute("y").as_int(),
			object_node.attribute("w").as_int(),
			object_node.attribute("h").as_int() };

		objects.add(CreateElement(
			img_rect,
			object_node.attribute("size").as_float(),
			object_node.attribute("type").as_int()));

		objects.end->data->Awake(object_node);

		object_node = object_node.next_sibling("obj");
	}
	


	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	bool ret = true;

	p2List_item<UI_Element*>* item = objects.start;

	for (int i = 0; i < objects.count(); i++) {
		if (item->data != nullptr) {
			ret = item->data->Start();
			item->data->point_atlas = atlas;
		}
		item = item->next;
	}

	return ret;

}

// Update all guis
bool j1Gui::PreUpdate(float dt)
{
	bool ret = true;

	p2List_item<UI_Element*>* item = objects.start;

	for (int i = 0; i < objects.count(); i++){
		if (item->data != nullptr && ((int)item->data->group == active_set || (int)item->data->group == all) ||
			((int)item->data->group == ui_set::inmenus && active_set != ui_set::ingame))
			ret = item->data->PreUpdate();
		item = item->next;
	}

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate(float dt)
{
	bool ret = true;

	p2List_item<UI_Element*>* item = objects.start;

	for (int i = 0; i < objects.count(); i++) {
		if (item->data != nullptr && ((int)item->data->group == active_set || (int)item->data->group == all) ||
			((int)item->data->group == ui_set::inmenus && active_set != ui_set::ingame))
			ret = item->data->PostUpdate();
		item = item->next;
	}

	ret = not_end;

	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	bool ret = true;

	ret = CleanUI();

	return ret;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

bool j1Gui::CleanUI()
{
	bool ret = true;

	p2List_item<UI_Element*>* item = objects.start;

	for (int i = 0; i < objects.count(); i++) {
		if(item->data != nullptr)
			ret = item->data->CleanUp();
		item = item->next;
	}

	objects.clear();

	return ret;
}

UI_Element* j1Gui::CreateElement(SDL_Rect& rect, float size, int type)
{
	switch (type) {
	case (int)label:
		return (new Label(rect,size));
	case (int)image:
		return (new Image(rect, size));
	case (int)textbox:
		return (new TextBox(rect, size));
	case (int)hyperlink:
		return (new HyperLink(rect, size));
	case (int)slider:
		return (new Slider(rect, size));
	case (int)backtomenu_b:
		return (new BackToMenu(rect, size));
	case (int)continue_b:
		return (new Continue(rect, size));
	case (int)credits_b:
		return (new Credits(rect, size));
	case (int)exit_b:
		return (new Exit(rect, size));
	case (int)settings_b:
		return (new Settings(rect, size));
	case (int)start_b:
		return (new Start_b(rect, size));
	case (int)volume_up:
		return (new VolumeUp(rect, size));
	case (int)volume_down:
		return (new VolumeDown(rect, size));
	default:
		return nullptr;
	}

	return nullptr;
}
// class Gui ---------------------------------------------------

