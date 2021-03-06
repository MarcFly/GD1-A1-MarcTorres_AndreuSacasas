#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;
	//img = App->tex->Load("textures/test.png");
	// LOAD MAPS AND MUSIC HERE
	ret = App->map->Load("TMX tests/hello.tmx");
	if(ret == true) ret = App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
	
	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	// TODO 2.5: Call load / save methods when pressing l/s

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->Trigger_Load();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->Trigger_Save();

	// TODO 2.Homework Allow for change in volume
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT) {
		App->audio->Increase_Master();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT) {
		App->audio->Decrease_Master();
	}

	// Camera movement Inputs

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 10;

	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	// TODO 3.7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2List_item<Map_info*>* tmp_map = App->map->Maps.start;
	p2SString title("Maps:%d MapSize:%dx%d TileSize:%dx%d Tilesets:%d Layers:%d Tiles:%d",
		App->map->Maps.count(),
		tmp_map->data->width, tmp_map->data->height,
		tmp_map->data->tilewidth, tmp_map->data->tileheight,
		tmp_map->data->tilesets.count(),
		tmp_map->data->layers.count(),
		tmp_map->data->layers.start->data->tiles.count());

	App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
