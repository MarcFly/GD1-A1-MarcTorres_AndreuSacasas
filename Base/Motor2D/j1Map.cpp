#include "p2Defs.h"
#include "p2Log.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include <string.h>

struct SDL_Texture;

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	/// TODO 3.6: Iterate all tilesets and draw all their 
	/// images in 0,0 (you should have only one tileset for now)

	// TODO 4.5 Loop to draw all tilesets + Blit
	// TODO 4.9 Complete draw function for layers and tiles

	iPoint pos = { 0,0 };

	if (Maps != nullptr) { //Check there is a map

		p2List_item<tileset_info*>* item_tileset = Maps->tilesets.start; //Start tileset list

		p2List_item<layer_info*>* item_layer = Maps->layers.start; //Start layer

		while (Maps->tilesets.count() > 1 && (*item_layer->data->data < item_tileset->data->firstgid || *item_layer->data->data > item_tileset->data->firstgid + item_tileset->data->tilecount))
			item_tileset = item_tileset->next; // This only works if layer only works over 1 tileset, if multiple tilesets in a layer, you have to do it before every blit?

		while (item_layer != nullptr) { //Check there are layers
			if (item_layer->data->draw_mode != 1) {
				uint* p = item_layer->data->data; // reset data pointing to data[0]

				for (int i = 0; i < item_layer->data->height; i++) {
					for (int j = 0; j < item_layer->data->width; j++) {
						App->render->MapBlit(
							item_tileset->data->image.tex,
							j * Maps->tilewidth,
							i * Maps->tileheight,
							&item_tileset->data->GetRect(*p));
						p++;
					}
				}
			}
			/*	else if (first_loop == true) {
					for (int i = 0; i < item_layer->data->height; i++) {
						for (int j = 0; j < item_layer->data->width; j++) {
							CreateGround(*item_layer->data, *item_tileset->data, j, i);  // Create a ground collider for such tile if required
						}
					}
				}*/

			item_layer = item_layer->next;
		}

		if (first_loop)
			first_loop = false;
	}
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 3.2: Make sure you clean up any memory allocated
	// from tilesets / map
	delete Maps;

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_document check_doc;

	pugi::xml_parse_result result = check_doc.load_file(tmp.GetString());

	if (Maps != nullptr)
		delete Maps;

	Maps = new Map_info();

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if (ret == true)
	{
		// TODO 3.3: Create and call a private function to load and fill
		// all your map data
		pugi::xml_node root_node = check_doc.child("map");

		LoadMapData(root_node, *Maps);
		
	}

	// TODO 3.4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!

	if (ret == true) {
		pugi::xml_node tileset_node = check_doc.child("map").child("tileset");

		if (tileset_node.attribute("firstgid").as_uint() != 0) {

			while (tileset_node.attribute("firstgid").as_int() > 0) {


				tileset_info* item_tileset = new tileset_info;
				ret = LoadTilesetData(tileset_node, *item_tileset);
				tileset_node = tileset_node.parent().next_sibling("tileset");


				Maps->tilesets.add(item_tileset);


			}

			map_loaded = true;
		}
		else {
			map_loaded = false;
			LOG("There is no tileset!/n");
		}

	}

	if (ret == true) {
		pugi::xml_node layer_node = check_doc.child("map").child("layer");

		while (layer_node.attribute("name").as_string() != "") {
			layer_info* item_layer = new layer_info;
			ret = LoadLayerData(layer_node, *item_layer);
			layer_node = layer_node.next_sibling("layer");
			Maps->layers.add(item_layer);

		}
	}

	if (ret == true)
	{
		// TODO 3.5: LOG all the data loaded
		// iterate all tilesets and LOG everything
	}

	ret = map_loaded;

	return ret;
}

bool j1Map::LoadMapData(const pugi::xml_node& map_node, Map_info& item_map) {
	bool ret = true;

	item_map.scale = map_node.attribute("scale").as_uint();
	// Orientation
	p2SString cmp = map_node.attribute("orientation").as_string();

	if (cmp.GetString() == "orthogonal") item_map.map_type = orthogonal;
	else if (cmp.GetString() == "isometric") item_map.map_type = isometric;
	else if (cmp.GetString() == "staggered") item_map.map_type = staggered;
	else if (cmp.GetString() == "hexagonal") item_map.map_type = hexagonal;
	else item_map.map_type = unknown_;

	// Renderorder
	cmp.create(map_node.attribute("renderorder").as_string());

	if (cmp.GetString() == "right-down") item_map.renderorder = right_down;
	if (cmp.GetString() == "right-up") item_map.renderorder = right_up;
	if (cmp.GetString() == "left-down") item_map.renderorder = left_down;
	if (cmp.GetString() == "left-up") item_map.renderorder = left_up;
	else item_map.renderorder = unknown;

	item_map.start_pos.x = map_node.attribute("start_y").as_int();
	item_map.start_pos.y = map_node.attribute("start_x").as_int();

	item_map.width = map_node.attribute("width").as_uint();
	item_map.height = map_node.attribute("height").as_uint();
	item_map.tilewidth = map_node.attribute("tilewidth").as_uint();
	item_map.tileheight = map_node.attribute("tileheight").as_uint();
	item_map.nextobjectid = map_node.attribute("nextobjectid").as_uint();

	return ret;
}

bool j1Map::LoadTilesetData(const pugi::xml_node& tileset_node, tileset_info& item_tileset) {
	bool ret = true;

	// Tileset info
	item_tileset.firstgid = tileset_node.attribute("firstgid").as_uint();
	item_tileset.name = tileset_node.attribute("name").as_string();
	item_tileset.tilewidth = tileset_node.attribute("tilewidth").as_uint();
	item_tileset.tileheight = tileset_node.attribute("tileheight").as_uint();
	item_tileset.spacing = tileset_node.attribute("spacing").as_uint();
	item_tileset.margin = tileset_node.attribute("margin").as_uint();

	item_tileset.tilecount = tileset_node.attribute("tilecount").as_uint();

	item_tileset.columns = tileset_node.attribute("columns").as_uint();

	// Load terrains
	for (int i = 1; i <= item_tileset.tilecount; i++) {

		terrain_info* item_terrain = new terrain_info;
		ret = LoadTerrainData(tileset_node, i, *item_terrain, item_tileset);
		item_tileset.terrains.add(item_terrain);

	}

	// Image info
	item_tileset.image.image_source.create(tileset_node.child("image").attribute("source").as_string());
	item_tileset.image.image_width = tileset_node.child("image").attribute("width").as_uint();
	item_tileset.image.image_height = tileset_node.child("image").attribute("height").as_uint();

	item_tileset.image.tex = App->tex->Load(item_tileset.image.image_source.GetString());

	return ret;
}

// Load Terrain Data-----------------------------------------------------------------------------------------------------------
bool j1Map::LoadTerrainData(const pugi::xml_node& tileset_node, const int& id, terrain_info& item_terrain, tileset_info& item_tileset) {

	item_terrain.id = id;

	item_terrain.Tex_Pos = new SDL_Rect(item_tileset.GetRect(item_terrain.id));

	// Create a switch depending on Id to create later collision data for later creation
	// There should be a way of detecting which type of layer we are in, and then load properties, no matter what type it is
	// Then load different properties depending on it, how tho?
	// Recommended with an int and just do a switch to choose laoding functions, no enum needed (you can comment what type of layer it is)
	// Or do with string and cmp (more memory tho)
	// switch(
	//pugi::xpath_node_set terrain_node = tileset_node.select_node("/tile[@id='%d']", id); Test the select node method to load a specific node instead of iterating to find
	return true;
}

// Load Layer Data-----------------------------------------------------------------------------------------------------------
bool j1Map::LoadLayerData(const pugi::xml_node& layer_node, layer_info& item_layer) {

	item_layer.name = layer_node.attribute("name").as_string();
	item_layer.width = layer_node.attribute("width").as_uint();
	item_layer.height = layer_node.attribute("height").as_uint();

	item_layer.draw_mode = layer_node.child("properties").child("property").attribute("value").as_uint();

	//Load all tiles in layer data
	pugi::xml_node tile_node = layer_node.child("data").child("tile");
	pugi::xml_node gid_check = layer_node.previous_sibling("tileset").previous_sibling("tileset").last_child();


	if (tile_node.attribute("gid").as_int() <= -1) LOG("There are no valid tiles, RETARDTIST!!!!\n");

	item_layer.size = item_layer.width * item_layer.height;
	item_layer.data = new uint[item_layer.size];

	uint* p = item_layer.data; // TO go through a "pointer" list you have to go through while going in another pointer so you are not changing where its pointing to the original one but giving the value
	for (int i = 0; i < item_layer.size; i++) {
		*p = tile_node.attribute("gid").as_uint();
		tile_node = tile_node.next_sibling("tile");
		p++;
	}

	return true;
}

iPoint j1Map::MapToWorld(int x, int y) const {

	return { 0,0 };
}
