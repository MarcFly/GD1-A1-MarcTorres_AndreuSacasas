#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UI_Elements.h"

class Label;
class Image;
class TextBox;
class Button;
class HyperLink;
class Slider;

class BackToMenu;
class Continue;
class Credits;
class Exit;
class Settings;
class Start_b;

class Timer;

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
enum element_type {
	error_ = 0,
	label,
	image,
	textbox,
	hyperlink,
	slider,

	backtomenu_b,
	continue_b,
	credits_b,
	exit_b,
	settings_b,
	start_b,
	volume_up,
	volume_down,

	timer,

	element_max = INT_MAX

};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(const pugi::xml_node& config);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called after all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	bool CleanUI();

	// TODO 2: Create the factory methods
	// Gui creation functions
	UI_Element* CreateElement(SDL_Rect& rect, float size, int type);

	bool DestroyElement(UI_Element* item)
	{
		int at = objects.find(item);
		objects.At(at)->data->CleanUp();
		objects.At(at)->data = nullptr;

	};

	const SDL_Texture* GetAtlas() const;

	void Set_ActiveSet(int set)
	{
		active_set = set;
	}

	int Get_ActiveSet()
	{
		return active_set;
	}

public:
	bool not_end = true;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	int active_set = 0;
	p2List<UI_Element*> objects;
};

#endif // __j1GUI_H__