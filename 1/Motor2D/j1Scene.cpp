#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "EntityManager.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(const pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	pugi::xml_node node = config.child("map");
	while (node.attribute("source").as_string() != "") {
		p2SString* new_map = new p2SString;
		new_map->create(node.attribute("source").as_string());
		Map_list.add(new_map);
		node = node.next_sibling("map");
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;
	// LOAD MAPS AND MUSIC HERE
	if (ret == true) ret = App->map->LoadMap(Map_list.start->data->GetString());
	curr_map = 0;
	if (ret == true) ret = App->audio->PlayMusic("audio/music/music_sadpiano.ogg");

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Get Mouse Position
	iPoint pos;
	App->input->GetMousePosition(pos.x, pos.y);
	pos = App->map->WorldToMap(pos.x - App->render->camera.x, pos.y - App->render->camera.y);
	
	
	if (god_mode)
		GodMode(dt);
	else
		NotGodMode(dt);

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		god_mode = !god_mode;
		App->render->offset_camera = 0;
	}
	
	//Check world position with the mouse

	//if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	//{
	//	int x = 0;
	//	int y = 0;
	//	App->input->GetMouseWorldPosition(x, y);
	//	x = x + y;
	//}

	// Draw all
	App->map->Draw();
	App->entities->Draw(dt);
	App->collisions->DebugDraw();

	// TODO 3.7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	
	p2SString title("MapSize:%dx%d TileSize:%dx%d Tilesets:%d Layers:%d Tiles:%d Position:%d %d",
		App->map->Maps->width, App->map->Maps->height,
		App->map->Maps->tilewidth, App->map->Maps->tileheight,
		App->map->Maps->tilesets.count(),
		App->map->Maps->layers.count(),
		App->map->Maps->layers.start->data->size,
		pos.x,pos.y
		);
	
	//App->win->SetTitle(title.GetString());
	
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		App->map->debug_draw = !App->map->debug_draw;
		App->collisions->debug = !App->collisions->debug;
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		App->ChangeCap();
		App->CapFps(App->GetFpsCap());
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate(float dt)
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

void j1Scene::LoadNextMap()
{
	App->map->EraseMap();
	App->entities->CleanEntities();
	App->pathfinding->ResetNav();
	App->collisions->CleanColliders();

	if (Map_list.At(curr_map)->next != NULL) {
		App->map->LoadMap(Map_list[curr_map + 1]->GetString());
		App->map->first_loop = true;
		curr_map++;
	}
	else
	{
		App->map->LoadMap(Map_list.start->data->GetString());
		curr_map = 0;
		App->map->first_loop = true;
	}

	App->entities->LoadEntities();
}

void j1Scene::LoadMap(const int& at)
{
	App->map->EraseMap();
	App->entities->CleanEntities();
	App->pathfinding->ResetNav();
	App->collisions->CleanColliders();

	if (Map_list.At(at) != NULL) {
		App->map->LoadMap(Map_list[at]->GetString());
		App->map->first_loop = true;
	}

	App->entities->LoadEntities();
}

void j1Scene::GodMode(float dt)
{
	// Camera movement Inputs
	// TODO 10.6: Make the camera movement independent of framerate

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 12 * dt;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 12 * dt;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->offset_camera += 12 * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->offset_camera -= 12 * dt;

	// Extra inputs for assignment
}

void j1Scene::NotGodMode(float dt) 
{
	// TODO 2.5: Call load / save methods when pressing l/s

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		curr_map = 0;
		LoadMap(curr_map);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		LoadMap(curr_map);

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN || (App->entities->GetPlayer() != nullptr && App->entities->GetPlayer()->stats.hp == 0))
		App->Trigger_Load();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->Trigger_Save();



	// TODO 2.Homework Allow for change in volume
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT) {
		App->audio->Increase_Master();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT) {
		App->audio->Decrease_Master();
	}

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN || change_map == true) {
		change_map = false;
		LoadNextMap();
	}
}