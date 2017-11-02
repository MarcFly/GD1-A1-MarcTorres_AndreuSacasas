#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "SDL/include/SDL.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Textures.h"

struct SDL_Rect;
struct SDL_Color;

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

// TODO 4.1 Create a struct for map layer
struct layer_info {
	p2SString	name;
	uint		width;	// Width in tiles of the layer
	uint		height;	// Height in tiles of layer
	uint		tilecount;

	int draw_mode; // 0 = Default, 1 = Collisions, 2 = Navigation

	// Data storing Tiles system
	uint*	data = nullptr;
	uint	size = 0;

	//TODO 4.6 Return X,Y of tile in tileset
	iPoint GetMapPos(int nid, int tile_width, int tile_height) {
		return { (int)(tile_width * ((nid) - (width*(nid / width)))), (int)(tile_height * (nid / width))};
	}

	inline int Get(const int& x, const int& y) { return data[y*width + x]; }

	~layer_info() {
		delete[] data;
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
	iPoint tileoffset = { 0,0 }; //Specific spacing declared in tiled
	uint spacing = 0; //Global spacing that applies to H and W
	uint margin;
	
	uint tilecount;
	uint columns;	// Amount of tiles (terrains) per row

	Image image;

	p2List<terrain_info*> terrains;

	// TODO 4.7 Method that gives the Rect given gid
	inline SDL_Rect GetRect(int gid) const {
		int w = tilewidth;
		int h = tileheight;
		int x = margin + (((gid - firstgid) % columns)*(w + spacing));
		int y = margin + (((gid - firstgid) / columns)*(h + spacing));
		
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
	//SDL_Color	bg_color;

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
	bool Awake(const pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// TODO 4.8 Method to translate map to world coordinates?
	iPoint MapToWorld(int x, int y) const;

	// TODO 4.9? Translate world to map coordinates
	iPoint WorldToMap(int rx, int ry) const;

	void DrawNav();
	void DrawPath();
	void DrawNPath();

	// Find Tileset
	tileset_info* GetTilesetFromTileId(int gid) const;

private:

	// TODO 3.3.2 Functions/Methods to load map data
	bool LoadMapData(const pugi::xml_node& root_node, Map_info& item);
	bool LoadTilesetData(const pugi::xml_node& data_node, tileset_info& item_tileset);

	// TODO 3.Homework Load terrains
	// Load Terrains
	bool LoadTerrainData(const pugi::xml_node& tileset_node, const int& id, terrain_info& item_terrain, tileset_info& item_tileset);

	// TODO 4.3 Load Layers
	bool LoadLayerData(const pugi::xml_node& layer_node, layer_info& item_layer);
	
	//void CreateCollider(layer_info& item_layer, tileset_info& item_tileset, int y, int x);

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