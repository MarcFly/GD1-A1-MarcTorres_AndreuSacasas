#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collisions.h"


struct cam_settings {
	uint	min_sep;		//Minimum separation of players to start making screen bigger (in pixels)
	uint	max_sep;		//Maximum separation of players which stops making screen bigger (in pixels)
	uint	scaling_step;	//Minimum 1, reccomend maximum 5, what you feel is better

	uint	screen_ratio;
};

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node* config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called when triggered
	bool Load(pugi::xml_node* savegame);

	bool Save(pugi::xml_node* savegame);


	// Called before quitting
	bool CleanUp();

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float scale = 1, double angle = 0, float speed = 1.0f, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool MapBlit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float scale = 1, double angle = 0, float speed = 1.0f, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool FlipBlit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float scale = 1, double angle = 0, float speed = 1.0f, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set Camera Distance
	bool SetCamDistance(const iPoint& curr_vec);

	// Set background color
	void SetBackgroundColor(SDL_Color color);

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	cam_settings    settings;
	SDL_Rect		viewport;
	SDL_Color		background;
};

#endif // __j1RENDER_H__