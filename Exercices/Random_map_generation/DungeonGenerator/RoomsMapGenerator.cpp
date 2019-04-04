#include "Defs.h"
#include "Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "RoomsMapGenerator.h"
#include "j1Input.h"
#include "j1Pathfinding.h"

#include "Brofiler\Brofiler.h"

#include <time.h>
#include <algorithm>

#define FLOOR { 0,0,32,16 }
#define WATER { 0,16,32,16 }
#define WALL { 32,0,32,16 }
RoomsMapGenerator::RoomsMapGenerator()
{}

// Destructor
RoomsMapGenerator::~RoomsMapGenerator()
{}


Room::~Room()
{	
	north.clear();

	south.clear();

	est.clear();
	
	west.clear();

	startPoints.clear();
}

// Called before render is available
bool RoomsMapGenerator::Awake(pugi::xml_node& config)
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool RoomsMapGenerator::CleanUp()
{
	bool ret = true;
	return ret;
}

// Get the position of a 1 dimensional container from a 2 dimensional parameter
inline int RoomsMapGenerator::Get(int x, int y) const
{
	return (map.gridSize.x * y + x);
}

void RoomsMapGenerator::SetMap(RoomsMapInfo& map)
{
	this->map = map;
	this->map.pathOffset = max(map.pathOffset, 1);
	this->map.rooms = max(map.rooms, 1);
	this->map.roomsMinMaxSize.x = max(map.roomsMinMaxSize.x, 3);
	this->map.roomsMinMaxSize.y = max(map.roomsMinMaxSize.y, 3);
	this->map.roomsMinMaxSize.w = max(map.roomsMinMaxSize.w, 3);
	this->map.roomsMinMaxSize.h = max(map.roomsMinMaxSize.h, 3);

}

// Get the position of the world from a map pos
iPoint RoomsMapGenerator::MapToWorld(iPoint xy) const
{
	iPoint ret;

	ret.x = xy.x * map.tileSize;
	ret.y = xy.y * map.tileSize;

	return ret;
}

// Get the position of the map from the world pos
iPoint RoomsMapGenerator::WorldToMap(int x, int y) const
{
	iPoint ret;

	ret.x = x / map.tileSize;
	ret.y = y / map.tileSize;

	return ret;
}

bool RoomsMapGenerator::GenerateDungeon(RoomsMapInfo& map) 
{
	BROFILER_CATEGORY("RoomsMapGeneration", Profiler::Color::Chocolate);

	bool ret = true;

	SetMap(map);
	
	// Load Texture
	//You can load it through xml mapData
	texture = App->tex->Load("tilesets/isometricMap.png");

	for (int i = 0; i < this->map.gridSize.y; ++i) {
		for (int j = 0; j < this->map.gridSize.x; ++j) {
			open.push_back(new DungeonNode(iPoint{ j,i }, WATER));
		}
	}
	if (open.size() != this->map.gridSize.x * this->map.gridSize.y)
		ret = false;

	// Check if the user want to use a seed and start srand()
	if (this->map.seed != 0)
		srand(this->map.seed);
	else
		srand(time(NULL));

	std::list<DungeonNode*> toCreateRooms;

	// Create a list copy from the open vector
	for (uint i = 0u; i < open.size() && ret; ++i)
		toCreateRooms.push_back(open[i]);

	for (int i = 1; i <= this->map.rooms && ret; ++i)
	{
		// Find a random pos
		int x = rand() % toCreateRooms.size();
		std::list<DungeonNode*>::iterator it = std::next(toCreateRooms.begin(), x);
		toCreateRooms.remove((*it));

		iPoint roomSize;

		roomSize.x = rand() % (this->map.roomsMinMaxSize.y + 1 - this->map.roomsMinMaxSize.x) + this->map.roomsMinMaxSize.x;
		roomSize.y = rand() % (this->map.roomsMinMaxSize.h + 1 - this->map.roomsMinMaxSize.w) + this->map.roomsMinMaxSize.w;

		if (!GenerateRoom((*it)->position, roomSize, i))
		{
			i--;
		}
	}

	toCreateRooms.clear();

	for (int i = 0; i < closed.size() - 1 && ret; ++i) {
		for (int x = i + 1; x < closed.size(); ++x) {
			if (closed[i]->position.x > closed[x]->position.x)
			{
				Room* aux = closed[i];
				closed[i] = closed[x];
				closed[x] = aux;
			}
		}
	}
	
	// Set walkability for every element in the closed vector
	for (int i = 0; i < closed.size() && ret; ++i)
		SetWalkability(closed[i]);

	// Generate corridors between every room using A* 
	if (ret)
		ret = GenerateCorridorsBetweenRooms();

	// Generate Walls for every room and corridor
	if (ret)
		ret = GenerateWalls();
	
	return ret;
}

// Check if the position is valid
bool RoomsMapGenerator::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x < (int)map.gridSize.x * map.tileSize &&
		pos.y >= 0 && pos.y < (int)map.gridSize.y * map.tileSize);
}

// Set the walkability for a room
bool RoomsMapGenerator::SetWalkability(Room* room)
{
	BROFILER_CATEGORY("RoomsMapGeneration -- Set Walkability", Profiler::Color::Chocolate);

	int offset = map.pathOffset;
	int validNode = rand() % (room->north.size() - 1);

	for (int i = 0; i < room->north.size(); ++i) {
		for (int j = 1; j <= offset; ++j)
		{
			if (i == validNode) {
				open[Get(room->north[i]->position.x, room->north[i]->position.y - j)]->InvalidWalkability = false;
				if (j == 1)
					room->startPoints.push_back(open[Get(room->north[i]->position.x, room->north[i]->position.y - 1)]);
			}
			else
				open[Get(room->north[i]->position.x, room->north[i]->position.y - j)]->InvalidWalkability = true;
		}
	}

	for (int i = 1; i <= offset; ++i) {
		for (int j = 1; j <= offset; ++j)
		{
			open[Get(room->north[0]->position.x - j, room->north[0]->position.y - i)]->InvalidWalkability = true;
			open[Get(room->north[room->north.size() - 1]->position.x + j, room->north[room->north.size() - 1]->position.y - i)]->InvalidWalkability = true;
		}
	}

	validNode = rand() % (room->south.size() - 1);

	for (int i = 0; i < room->south.size(); ++i) {
		for (int j = 1; j <= offset; ++j)
		{
			if (i == validNode) {
				open[Get(room->south[i]->position.x, room->south[i]->position.y + j)]->InvalidWalkability = false;
				if (j == 1)
					room->startPoints.push_back(open[Get(room->south[i]->position.x, room->south[i]->position.y + 1)]);
			}
			else
				open[Get(room->south[i]->position.x, room->south[i]->position.y + j)]->InvalidWalkability = true;
		}
	}

	for (int i = 1; i <= offset; ++i) {
		for (int j = 1; j <= offset; ++j)
		{
			open[Get(room->south[0]->position.x - j, room->south[0]->position.y + i)]->InvalidWalkability = true;
			open[Get(room->south[room->south.size() - 1]->position.x + j, room->south[room->south.size() - 1]->position.y + i)]->InvalidWalkability = true;
		}
	}

	validNode = rand() % (room->west.size() - 1);

	for (int i = 0; i < room->west.size(); ++i) {
		for (int j = 1; j <= offset; ++j)
		{
			if (i == validNode) {
				open[Get(room->west[i]->position.x - j, room->west[i]->position.y)]->InvalidWalkability = false;
				if (j == 1)
					room->startPoints.push_back(open[Get(room->west[i]->position.x - 1, room->west[i]->position.y)]);
			}
			else
				open[Get(room->west[i]->position.x - j, room->west[i]->position.y)]->InvalidWalkability = true;
		}
	}

	validNode = rand() % (room->est.size() - 1);

	for (int i = 0; i < room->est.size(); ++i) {
		for (int j = 1; j <= offset; ++j)
		{
			if (i == validNode) {
				open[Get(room->est[i]->position.x + j, room->est[i]->position.y)]->InvalidWalkability = false;
				if (j == 1)
					room->startPoints.push_back(open[Get(room->est[i]->position.x + 1, room->est[i]->position.y)]);
			}
			else
				open[Get(room->est[i]->position.x + j, room->est[i]->position.y)]->InvalidWalkability = true;
		}
	}
	return true;
}

// Generate walls for every room and corridor
bool  RoomsMapGenerator::GenerateWalls()
{
	BROFILER_CATEGORY("RoomsMapGeneration -- Walls Generation", Profiler::Color::Chocolate);

	for (int i = 0; i < open.size(); ++i)
	{
		if (SDL_RectEquals((const SDL_Rect*)&open[i]->tileRect, &SDL_Rect(FLOOR)))
		{
			DungeonNode* node = open[i];

			/*TODO 3: Generate the walls comparing the visited FLOOR nodes to the current node. 
			If there is a WATER tile on the node perimeter then change it for a WALL.
			Check ALL perimeter directions.*/

			//Hint: It is very similar than the first Isometric Map. Take a look!
			//Utilities: You can use your tileset and replace it here!!

			if (SDL_RectEquals((const SDL_Rect*)&open[Get(node->position.x + 1, node->position.y)]->tileRect, &SDL_Rect(WATER)) && CheckBoundaries(MapToWorld({ node->position.x + 1, node->position.y })))
				open[Get(node->position.x + 1, node->position.y)]->tileRect = WALL;
		}
	}

	return true;
}

// Generate one Room :)
bool RoomsMapGenerator::GenerateRoom(iPoint& pos, iPoint& size, int room)
{
	BROFILER_CATEGORY("RoomsMapGeneration -- Rooms Generation", Profiler::Color::Chocolate);

	if (CheckBoundaries(MapToWorld({ pos.x, pos.y })))
	{
		// Check if the pos is valid
		for (int i = -map.roomsOffset; i < size.y + map.roomsOffset; ++i)
		{
			for (int j = -map.roomsOffset; j < size.x + map.roomsOffset; ++j)
			{
				if (!CheckBoundaries(MapToWorld({ pos.x + j, pos.y + i })) || SDL_RectEquals((const SDL_Rect*)&open[Get(pos.x + j, pos.y + i)]->tileRect, &SDL_Rect(FLOOR)))
					return false;
			}
		}

		// Create the room
		Room* room = new Room;

		for (int i = 0; i < size.y; ++i)
		{
			for (int j = 0; j < size.x; ++j)
			{
				DungeonNode* roomNode = open[Get(pos.x + j, pos.y + i)];
				if (i == 0 && j == 0)
					room->position = roomNode->position;

				if (i == 0)
					room->north.push_back(roomNode);
				else if (i == size.y - 1)
					room->south.push_back(roomNode);
				else if (j == 0)
					room->west.push_back(roomNode);
				else if (j == size.x - 1)
					room->est.push_back(roomNode);
				roomNode->tileRect = FLOOR;
				roomNode->InvalidWalkability = true;
			}
		}

		room->est.push_back(open[Get(room->est[0]->position.x, room->est[0]->position.y - 1)]);
		room->est.push_back(open[Get(room->south[room->south.size() - 1]->position.x, room->south[room->south.size() - 1]->position.y)]);
		room->west.push_back(open[Get(room->west[0]->position.x, room->west[0]->position.y - 1)]);
		room->west.push_back(open[Get(room->south[0]->position.x, room->south[0]->position.y)]);

		closed.push_back(room);

		return true;
	}

	return false;
}

// Generate Corridors between rooms using A*
bool RoomsMapGenerator::GenerateCorridorsBetweenRooms()
{
	BROFILER_CATEGORY("RoomsMapGeneration -- Corridors Generation", Profiler::Color::Chocolate);

	bool ret = true;

	for (int i = 0; i < closed.size(); ++i) {
		if (i != closed.size() - 1) {
			int pathPosition = rand() % (closed[i]->startPoints.size() - 1);
			int pathPositionEnd = rand() % (closed[i + 1]->startPoints.size() - 1);

			//TODO 2: Create a path with A* using the CreatePath() function. We want to connect rooms according to their X order.

		}
	}

	return ret;
}

// Clean the Dungeon
bool RoomsMapGenerator::CleanDungeon()
{
	BROFILER_CATEGORY("RoomsMapGeneration -- Dungeon Clean Up", Profiler::Color::Chocolate);

	bool ret = true;

	LOG("Cleaning Dungeon...");
	for (int i = 0; i < closed.size(); ++i)
	{
		delete closed[i];
		closed[i] = nullptr;
	}
	closed.clear();

	for (int i = 0; i < open.size(); ++i)
	{
		delete open[i];
		open[i] = nullptr;
	}

	open.clear();
	App->tex->UnLoad(texture);

	currentNode = nullptr;

	return ret;
}

// Draw the entire map
bool RoomsMapGenerator::Draw()
{
	BROFILER_CATEGORY("RoomsMapGeneration -- Draw", Profiler::Color::Chocolate);

	bool ret = true;

	std::vector<DungeonNode*>::iterator item;
	int first, second;

	for (item = open.begin(); item != open.end() && ret; ++item)
	{
		iPoint pos = MapToWorld((*item)->position);
		first = (pos.x - pos.y) * 16;
		second = (pos.x + pos.y) * 8;
		ret = App->render->Blit(texture, first, second, &(*item)->tileRect);
	}

	return ret;
}