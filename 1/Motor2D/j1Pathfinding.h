#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Queue.h"
#include "p2PQueue.h"
#include "p2Point.h"
#include "p2List.h"
#include "p2DynArray.h"
#include "j1Textures.h"


#define COST_MAP 100

struct PathList;

struct PathNode {
	iPoint	pos;
	uint	step_cost; // G
	uint	dist_cost; // H
	const PathNode* parent;

	// ----------
	// Methods

	PathNode() { step_cost = -1; dist_cost = -1; pos = { -1,-1 }; parent = NULL; }

	PathNode(uint g, uint h, const iPoint& pos_, const PathNode* parent_)
	{ step_cost = g; dist_cost = h; pos = pos_; parent = parent_;}

	PathNode(const PathNode& node) 
	{ step_cost = node.step_cost; dist_cost = node.dist_cost; pos = node.pos; parent = node.parent;}
	
	int	Score() const {
		return step_cost + dist_cost;
	}

	int CalculateF(const iPoint& dest) {
		step_cost = parent->step_cost + 1;
		dist_cost = pos.DistanceManhattan(dest);
		return Score();
	}

	uint FindWalkableAdjacents(PathList& list_to_fill) const;

	bool operator ==(const PathNode& node) const
	{
		return (pos == node.pos);
	}

	bool operator !=(const PathNode& node) const
	{
		return (pos != node.pos || step_cost != node.step_cost || dist_cost != node.dist_cost);
	}
	// 
};

struct PathList {
	p2List<PathNode>		nodes;

	// What?
	
	p2List_item<PathNode>* Find(const iPoint& point) {
		p2List_item<PathNode>* item = nodes.start;

		for (int i = 0; i < nodes.count(); i++)
		{
			if (item->data.pos == point)
				return item;
			item = item->next;
		}

		return NULL;
	}

	p2List_item<PathNode>* GetNodeLowestScore() const {
		p2List_item<PathNode>* ret = NULL;
		int min = 100000000;

		p2List_item<PathNode>* item = nodes.start;
		while (item) {
			if (item->data.Score() < min)
			{
				min = item->data.Score();
				ret = item;
			}
			item = item->next;
		}

		return ret;
	}
};

class j1Pathfinding : public j1Module
{
// Pathfinding

public:

	// Called before render is available
	bool Awake(const pugi::xml_node& conf);

	// Called before quitting
	bool CleanUp();

	// Set map info for usage
	void SetMap(uint width, uint height, uint* data);
	
	// Path things
	int CreateFPath(const iPoint& origin, const iPoint& dest);
	const p2DynArray<iPoint>* GetLastPath() const
	{
		return &last_path;
	}
	
	bool IsWalkable(int x, int y) const;

	bool CheckBoundaries(const iPoint& pos) const
	{
		return (pos.x > 0 && pos.x < width && 
			pos.y >= 0 && pos.y <= height);
	}

	uint GetTileAt(const iPoint& pos) const
	{
		if (CheckBoundaries(pos))
			return map[(pos.y*width) + pos.x];
	}


	void PropagateAStar(const iPoint& pos);

public:
	

	//Pathfinding Proper Bullshit
	PathList*	this_list;
	uint	width;
	uint	height;
	uint*	map;
	p2DynArray<iPoint> last_path;

	p2DynArray<iPoint> closed_test; // To show what A* does to find it, seems wrong for now
	//Pathfinding~ Hacked bullshit--------------------------------------------------------------------------------------------------------------
	iPoint				start;
	p2Queue<iPoint>		frontier;
	p2PQueue<iPoint>	pfrontier;
	p2List<iPoint>		visited;
	p2List<iPoint>		breadcrumbs;
	p2List<iPoint>		path;

	iPoint PropagateBFS();
	void PropagateToBFS(const iPoint& pos);

	iPoint PropagateDijkstra();
	void PropagateToDijkstra(const iPoint& pos);

	
	void ResetNav();
	void CreatePath(const iPoint& pos);
	int	 MovementCost(int x, int y) const;
	void SetStart(const iPoint& pos);

	uint				cost_so_far[COST_MAP][COST_MAP];

	SDL_Texture*		tile_x = nullptr;
};

#endif //__j1PATHFINDING_H__