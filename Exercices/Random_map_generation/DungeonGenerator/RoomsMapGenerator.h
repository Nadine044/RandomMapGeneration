#ifndef __RoomsMapGenerator_H__
#define __RoomsMapGenerator_H__

#include "PugiXml/src/pugixml.hpp"
#include "j1Module.h"
#include <vector>
#include "Point.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;

struct RoomsMapInfo
{
	iPoint gridSize = { 0,0 };
	uint rooms = 0u;
	int roomsOffset = 0u;
	SDL_Rect roomsMinMaxSize = { 0,0,0,0 };
	uint pathOffset = 0u;
	int seed = NULL;
	char* tilesetPath = nullptr;
	uint tileSize = 0u;
};

class DungeonNode
{
public:

	DungeonNode(iPoint position, SDL_Rect tileRect) :position(position), tileRect(tileRect) {};
	~DungeonNode() {};

public:

	iPoint position = { 0,0 };
	SDL_Rect tileRect = { 0,0,0,0 };
	int priority = 0;
	bool InvalidWalkability = false;
	int ups = 0;
};

class Room
{
public:
	Room() {};
	~Room();
public:
	std::vector<DungeonNode*> north;
	std::vector<DungeonNode*> south;
	std::vector<DungeonNode*> est;
	std::vector<DungeonNode*> west;

	iPoint position;

	std::vector<DungeonNode*> startPoints;

	uint roomIndex = 0u;
};

class RoomsMapGenerator
{
public:

	RoomsMapGenerator();

	// Destructor
	virtual ~RoomsMapGenerator();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before quitting
	bool CleanUp();

	inline int Get(int x, int y) const;

	iPoint MapToWorld(iPoint xy) const;

	iPoint WorldToMap(int x, int y) const;

	bool CheckBoundaries(const iPoint& pos) const;

	bool GenerateDungeon(RoomsMapInfo& map);

	bool GenerateRoom(iPoint& pos, iPoint& size, int room);

	bool GenerateCorridorsBetweenRooms();

	bool SetWalkability(Room* room);

	bool GenerateWalls();

	void SetMap(RoomsMapInfo& map);

	// Set all tiles to the default tile
	bool CleanDungeon();

	bool Draw();

public:

	std::vector<DungeonNode*> open;
	std::vector<Room*> closed;

	RoomsMapInfo map;
	SDL_Texture* texture = nullptr;
	DungeonNode* currentNode = nullptr;

};

#endif // __RoomsMapGenerator__