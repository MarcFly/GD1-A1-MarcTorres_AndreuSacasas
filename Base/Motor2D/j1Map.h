#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Textures.h"

// TODO 3.Homework
// Do layer printing
/*struct properties {
	
	struct property
	{
		p2SString name;
		bool value;
	};

	p2List<property*> porperty_list;

};*/

enum walk_types {
	unknown___ = -1,
	walkable,
	unwalkable,
	water,
	jumpable
	//keep adding types as you encounter them
};

struct map_tile_info {
	//properties properties;
	uint id;
	walk_types type;
};

struct layer_info {
	p2SString	name;
	uint		width;
	uint		height;

	p2List<map_tile_info*> tiles;

	~layer_info() {
		tiles.clear();
	}
};

struct terrain_info {
	uint id;

	SDL_Rect* Tex_Pos;
};

// TODO 3.2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct Image {
	SDL_Texture*	tex;
	const char*		image_source;
	uint			image_width;
	uint			image_height;
};

struct tileset_info {
	uint		firstgid;
	p2SString	name;
	
	uint tilewidth;
	uint tileheight;
	uint spacing;
	uint margin;
	
	uint tilecount;
	uint columns;

	Image image;

	p2List<terrain_info*> terrains;

	~tileset_info() {
		terrains.clear();
	}
	//SDL_Rect GetTileRect(int id) const;
	//tile_info* GetTileType(int tile_id) const {};
};

// TODO 3.1: Create a struct needed to hold the information to Map node
enum orientation {
	unknown_ = -1,
	orthogonal,
	isometric,
	staggered,
	hexagonal
};

enum renderorder {
	unknown = -1,
	right_down,
	right_up,
	left_down,
	left_up
};

struct Map_info {
	pugi::xml_document map_file;

	orientation	map_type;
	uint		renderorder;
	SDL_Color	bg_color;

	uint		width;
	uint		height;
	uint		tilewidth;
	uint		tileheight;

	uint	nextobjectid;

	p2List<tileset_info*> tilesets;
	p2List<layer_info*> layers;

	~Map_info() {
		tilesets.clear();
		layers.clear();
	}
	//Map_info() {};
	
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// Add a new map to list
	void AddMap(Map_info* name);

private:

	// TODO 3.3.2 Functions/Methods to load map data
	bool LoadMapData(Map_info* item,pugi::xml_node* root_node);
	bool LoadTilesetData(pugi::xml_node* data_node, tileset_info* item_tileset);

	// Load Terrains
	bool LoadTerrainData(const int& id, terrain_info* item_terrain, tileset_info* item_tileset);

	// Load Layers
	bool LoadLayerData(pugi::xml_node* layer_node, layer_info* item_layer);

	// Load Tiles
	bool LoadTileData(pugi::xml_node* tile_node, map_tile_info* item_tile);

public:

	// TODO 3.1: Add your struct for map info as public for now
	p2List<Map_info*> Maps;

	// MAPS that will be loaded
	Map_info*	hello;
	Map_info*	Hello2;

private:

	//pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__