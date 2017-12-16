#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"
#include "SDL\include\SDL.h"

#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define LAST_KEYS_PRESSED_BUFFER 50



struct SDL_Rect;

enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum j1KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class j1Input : public j1Module
{

public:

	j1Input();

	// Destructor
	virtual ~j1Input();

	// Called before render is available
	bool Awake(const pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(j1EventWindow ev);

	// Check if a certain window event happened
	bool GetWindowEvent(int id);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseWorldPosition(int & x, int & y);
	void GetMouseMotion(int& x, int& y);

	// Check key states (includes mouse and joy buttons)
	j1KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	j1KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}



	const char* GetTextBuffer()
	{
		return text_buffer.GetString();
	}

	void ClearTextBuffer()
	{
		text_buffer.Clear();
	}

	void StartBuffer() { SDL_StartTextInput(); }
	void StopBuffer() { SDL_StopTextInput(); }


private:
	void CleanKeys();

private:
	bool		windowEvents[WE_COUNT];
	j1KeyState*	keyboard;
	j1KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

	p2SString	text_buffer;
};

#endif // __j1INPUT_H__