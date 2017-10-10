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

// TODO 4.1 Create a struct for map layer
struct layer_info {
	p2SString	name;
	uint		width;	// Width in tiles of the layer
	uint		height;	// Height in tiles of layer

	uint		draw_mode; //0 = default, 1 = debug such as colliders, 2 = special?

	uint*	data = nullptr;
	uint	size = 0;

	p2List<SDL_Rect*> layer_coll;

	//TODO 4.6 Return X,Y of tile in tileset
	iPoint GetMapPos(int nid, int tile_width, int tile_height) {
		return { (int)(tile_width * ((nid)-(width*(nid / width)))), (int)(tile_height * (nid / width)) };
	}

	~layer_info() {
		delete[] data;

		for (int i = 0; i < layer_coll.count(); i++) {
			delete layer_coll[i];
		}
	}
};

// TODO 3.Homework Terrains
struct terrain_info {
	uint id;

	SDL_Rect* Tex_Pos;
};

// TODO 3.2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct Image {
	SDL_Texture*	tex;
	p2SString		image_source;
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
	uint columns;	// Amount of tiles (terrains) per row

	Image image;

	p2List<terrain_info*> terrains;

	// TODO 4.7 Method that gives the Rect given gid
	SDL_Rect GetRect(int gid) {
		int x = margin + (((gid - 1 - (columns * ((gid - 1) / columns)))*(tilewidth + spacing)));
		// Given Tileset starts at margin, we count from 0 because calculus
		// There are certain amount of columns that decide how many tiles occupy a row, so every X tiles (gid-1) it creates a new row -> (gid - 1) / Colums -> represent which row it belongs
		// - (columns*row) calculates how many tiles you have to go back to calculate from X = 0 for the X displacement in that row, for GID = 40 Columns = 8, GID is row 4, it will return 36 to wich it will calculate X offset for 3
		// (gid - 1 - (colums * row)) -> base tile to calculate X displacement in a row * (tilewidth + spacing) to measure in pixels where in the tileset it belongs
		int y = margin + (((gid - 1) / columns)*(tileheight + spacing)); //Calculate row and just add height and spacing repending on row (row 1 -> + 1 * (tileheight + spacing))
		int w = tilewidth;
		int h = tileheight;

		return { x,y,w,h };
	}

	~tileset_info() {
		for (int i = 0; i < terrains.count(); i++)
			delete terrains[i];
		terrains.clear();
	}
};

// TODO 3.1: Create a struct needed to hold the information to Map node, including tilesets
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

	uint scale;

	iPoint start_pos;

	uint		width;
	uint		height;
	uint		tilewidth;
	uint		tileheight;

	uint	nextobjectid;

	p2List<tileset_info*> tilesets;
	p2List<layer_info*> layers; // TODO 4.2 Layers list

	~Map_info() {
		for (int i = 0; i < tilesets.count(); i++)
			delete tilesets[i];
		tilesets.clear();

		for (int i = 0; i < layers.count(); i++)
			delete layers[i];
		layers.clear();
	}
	
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

	// TODO 4.8 Method to translate map to world coordinates?
	iPoint MapToWorld(int x, int y) const;

private:

	// TODO 3.3.2 Functions/Methods to load map data
	bool LoadMapData(const pugi::xml_node& root_node, Map_info& item);
	bool LoadTilesetData(const pugi::xml_node& data_node, tileset_info& item_tileset);

	// TODO 3.Homework Load terrains
	// Load Terrains
	bool LoadTerrainData(const pugi::xml_node& tileset_node, const int& id, terrain_info& item_terrain, tileset_info& item_tileset);

	// TODO 4.3 Load Layers
	bool LoadLayerData(const pugi::xml_node& layer_node, layer_info& item_layer);

	void CreateCollider(layer_info& item_layer, tileset_info& item_tileset, int y, int x);


public:

	// TODO 3.1: Add your struct for map info as public for now
	Map_info* Maps;
	bool first_loop = true;

private:

	//pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__