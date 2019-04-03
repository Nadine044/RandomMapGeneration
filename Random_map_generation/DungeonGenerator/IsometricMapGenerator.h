#ifndef __ISOMETRICMAPGENERATOR_H__
#define __ISOMETRICMAPGENERATOR_H__

#include "SDL\include\SDL_rect.h"
#include "Point.h"
#include <vector>
#include <list>

struct SDL_Texture;

class Module;

struct IsometricMapData
{
	char* tilesetPath = NULL;
	int sizeX = 0;
	int sizeY = 0;	
	uint iterations = 0u;
	uint tileSize = 0u;
	int seed = 0;
};

struct IsometricMapNode
{
	IsometricMapNode(iPoint pos, SDL_Rect whatToBlit) : pos(pos), whatToBlit(whatToBlit) {};
	~IsometricMapNode() {};

	iPoint pos = { 0,0 };
	int cost = -1;
	SDL_Rect whatToBlit = { 0,0,0,0 };
};

class IsometricMapGenerator
{
public:
	// Create a new tag if you want more kind of walls
	enum class nodeType
	{
		noType = -1,
		typeTiledWall,
		typeWall,
		typeFully
	};

public:

	IsometricMapGenerator();
	~IsometricMapGenerator();
	bool DrawMap();
	inline int Get(int x, int y) const;
	bool CheckBoundaries(const iPoint& pos) const;
	bool GenerateMap(IsometricMapData& data);
	bool ExecuteAlgorithm(IsometricMapNode* startNode, IsometricMapData& data);
	int CheckTypeOfNode(IsometricMapNode* nodetocheck);
	void UpdateNode(IsometricMapNode* nodetocheck, int type);
	bool GenerateWalls();
	bool CleanUp();
	void getSize(uint& w, uint& h) const;

private:

	uint totalSize = 0u;
	uint sizeX = 0u;
	uint sizeY = 0u;
	std::vector<IsometricMapNode*> nodes;
	std::vector<IsometricMapNode*> visited;
	SDL_Texture* mapTexture = nullptr;
};

#endif
