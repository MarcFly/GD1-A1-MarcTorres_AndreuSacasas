#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "p2Log.h"

/// PATHFINDING
j1Pathfinding::j1Pathfinding()
{
	name.create("pathfinding");
}

bool j1Pathfinding::Awake(const pugi::xml_node& config) {
	bool ret = true;

	start = { 19,4 };
	ResetNav(); // Start BFS

	return ret;
}

bool j1Pathfinding::CleanUp() {
	LOG("Freeing pathfinding");

	ResetNav();
	RELEASE_ARRAY(map);

	// Hacked Shite------------------------
	if (tile_x != nullptr)
		App->tex->UnLoad(tile_x);

	return true;
}

void j1Pathfinding::SetMap(uint width, uint height, uint* data) {
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uint[width*height];
	memcpy(map, data, width*height);
}


int j1Pathfinding::CreateFPath(const iPoint& origin, const iPoint& dest) {
	int ret = -1;
	ResetNav();
	if (IsWalkable(origin.x, origin.y) && IsWalkable(dest.x, dest.y)) {
		ret = 0;

		PathList open;
		PathList closed;

		open.nodes.add(PathNode(0,origin.DistanceTo(dest),origin,NULL));

		while (open.nodes.count() != 0) {
			PathNode* curr = new PathNode(open.GetNodeLowestScore()->data);
			open.nodes.del(open.GetNodeLowestScore());
			closed.nodes.add(*curr);

			if (curr->pos != dest) {
				PathList neighbours;
				curr->FindWalkableAdjacents(neighbours);

				for (int i = 0; i < neighbours.nodes.count(); i++) {
					if (closed.Find(neighbours.nodes.At(i)->data.pos) == NULL) {
						neighbours.nodes[i].CalculateF(dest);

						if (open.Find(neighbours.nodes[i].pos) != NULL) {
							if (open.Find(neighbours.nodes[i].pos)->data.step_cost > neighbours.nodes[i].step_cost)
								open.Find(neighbours.nodes[i].pos)->data.parent = neighbours.nodes[i].parent;
						}
						else 
							open.nodes.add(neighbours.nodes[i]);
					}
				}
				
			}
			else
				break;
			
			curr = nullptr;

		}

		p2List_item<PathNode>* item = closed.nodes.end;

		last_path.Clear();

		while (item->data.pos != origin)
		{
			last_path.PushBack(item->data.pos);
			item = closed.nodes.At(closed.nodes.find(*item->data.parent));
		}

		last_path.Flip();

		// Closed Test is to show what has A* done to find the path
		item = closed.nodes.start;

		for (int i = 0; i < closed_test.Count(); i++)
			delete closed_test.At(i);
		closed_test.Clear();

		while (item != nullptr)
		{
			closed_test.PushBack(item->data.pos);
			item = item->next;
		}

		open.nodes.clear();
		closed.nodes.clear();
	}

	

	return ret;
	
}

// Cutre Version for bfs
bool j1Pathfinding::IsWalkable(int x, int y) const {
	bool ret = true;

	ret = (x >= 0 && x < App->map->Maps->width &&
		y >= 0 && y < App->map->Maps->height);

	if (ret == true) {
		p2List_item<layer_info*>* item = App->map->Maps->layers.start;
		while (item->data->draw_mode != 1)
			item = item->next;

		ret = (item->data->data[y * App->map->Maps->width + x] == 0);
	}

	return ret;
}

uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const {
	iPoint cell(0, 0);
	uint before = list_to_fill.nodes.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell.x, cell.y))
		list_to_fill.nodes.add(PathNode(-1,-1,cell,this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell.x, cell.y))
		list_to_fill.nodes.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell.x, cell.y))
		list_to_fill.nodes.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell.x, cell.y))
		list_to_fill.nodes.add(PathNode(-1, -1, cell, this));

	/*// TODO 8.Homework
	// north east
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell.x, cell.y))
		list_to_fill.nodes.add(PathNode(-1, -1, cell, this));

	// south east
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell.x, cell.y))
		list_to_fill.nodes.add(PathNode(-1, -1, cell, this));

	// north west
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell.x, cell.y))
		list_to_fill.nodes.add(PathNode(-1, -1, cell, this));

	// south west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell.x, cell.y))
		list_to_fill.nodes.add(PathNode(-1, -1, cell, this));
		*/
	return list_to_fill.nodes.count();
}

// Settings makers
void j1Pathfinding::ResetNav() {
	last_path.Clear();

	closed_test.Clear();
	
	for (int i = 0; i < COST_MAP; i++)
		for (int j = 0; j < COST_MAP; j++)
			cost_so_far[i][j] = 0;
}

void j1Pathfinding::SetStart(const iPoint& pos) {
	if (IsWalkable(pos.x, pos.y))
		start = pos;
}

int	 j1Pathfinding::MovementCost(int x, int y) const {

	int ret = -1;

	if (x >= 0 && x < App->map->Maps->width && y >= 0 && y < App->map->Maps->height)
	{
		int id = App->map->Maps->layers.start->next->data->Get(x, y);

		if (id == 0)
			ret = 3;
		else
			ret = -1;
	}

	return ret;
}