#include "j1App.h"
#include "IsometricMapGenerator.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Log.h"

#include "Brofiler\Brofiler.h"

#include <time.h>

#define FLOOR { 0,0,32,16 }
#define WATER { 0,16,32,16 }
#define WALL { 32,0,32,16 }
#define START { 32,32,32,16 }

IsometricMapGenerator::IsometricMapGenerator() {}

IsometricMapGenerator::~IsometricMapGenerator() {}

bool IsometricMapGenerator::GenerateMap(IsometricMapData& data)
{
	BROFILER_CATEGORY("IsometricMapGeneration", Profiler::Color::Azure);

	bool ret = true;

	mapTexture = App->tex->Load("tilesets/isometricMap.png");

	this->sizeX = data.sizeX;
	this->sizeY = data.sizeY;
	totalSize = sizeX * sizeY;

	for (int i = 0; i < sizeY; ++i)
	{
		for (int j = 0; j < sizeX; ++j)
		{
			nodes.push_back(new IsometricMapNode({ j, i }, WATER));
		}
	}
	ret = nodes.size() == totalSize;

	if (ret)
		ret = ExecuteAlgorithm(nodes[Get(sizeX / 2, sizeY / 2)], data);

	if (ret)
		ret = GenerateWalls();

	return ret;
}

bool IsometricMapGenerator::CleanUp()
{
	BROFILER_CATEGORY("IsometricMapGeneration -- CleanUp", Profiler::Color::Azure);

	bool ret = true;

	LOG("Unloading Map Texture...");
	ret = App->tex->UnLoad(mapTexture);

	for (int i = 0; i < sizeX; ++i)
	{
		for (int j = 0; j < sizeY; ++j)
		{
			RELEASE(nodes[i]);
		}
	}

	nodes.clear();

	visited.clear();

	totalSize = 0u;
	sizeX = 0u;
	sizeY = 0u;
	//tileSize = 0u;

	return nodes.size() <= 0 && visited.size() <= 0 && ret;
}

bool IsometricMapGenerator::DrawMap()
{
	BROFILER_CATEGORY("IsometricMapGeneration -- Draw", Profiler::Color::Azure);

	bool ret = true;
	float first, second;
	
	for (uint i = 0u; i < nodes.size() && ret; ++i)
	{
		iPoint posToBlit = nodes[i]->pos;
		first = (posToBlit.x - posToBlit.y) * 16;
		second = (posToBlit.x + posToBlit.y) * 8;
		ret = App->render->Blit(mapTexture, first, second, &nodes[i]->whatToBlit);
	}
	return ret;
}

inline int IsometricMapGenerator::Get(int x, int y) const
{
	return (sizeX * y + x);
}

bool IsometricMapGenerator::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x > 1 && pos.x < (int)sizeX - 2 &&
		pos.y > 1 && pos.y < (int)sizeY - 2);
}

bool IsometricMapGenerator::ExecuteAlgorithm(IsometricMapNode* startNode, IsometricMapData& data)
{
	BROFILER_CATEGORY("IsometricMapGeneration -- ExecuteAlgorithm", Profiler::Color::Azure);

	LOG("Executing map Algorithm...");

	if (data.seed != 0)
		srand(data.seed);
	else
		srand(time(NULL));

	startNode->whatToBlit = FLOOR;
	visited.push_back(startNode);

	IsometricMapNode* auxNode = startNode;

	for (uint i = 0u; i < data.iterations;)
	{
		int nextTilePerCent = rand() % 100;

		if (nextTilePerCent <= 25 && CheckBoundaries({ auxNode->pos.x + 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)];
			if (!SDL_RectEquals(&auxNode->whatToBlit, &SDL_Rect(FLOOR)))
			{
				auxNode->whatToBlit = FLOOR;
				auxNode->cost = 0;
				visited.push_back(auxNode);
				i++;
			}
		}

		else if (nextTilePerCent <= 50 && CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)];
			if (!SDL_RectEquals(&auxNode->whatToBlit, &SDL_Rect(FLOOR)))
			{
				auxNode->whatToBlit = FLOOR;
				auxNode->cost = 0;
				visited.push_back(auxNode);
				i++;
			}
		}

		else if (nextTilePerCent <= 75  && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)];
			if (!SDL_RectEquals(&auxNode->whatToBlit, &SDL_Rect(FLOOR)))
			{
				auxNode->whatToBlit = FLOOR;
				auxNode->cost = 0;
				visited.push_back(auxNode);
				i++;
			}
		}

		else if (nextTilePerCent <= 100  && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)];
			if (!SDL_RectEquals(&auxNode->whatToBlit, &SDL_Rect(FLOOR)))
			{
				auxNode->whatToBlit = FLOOR;
				auxNode->cost = 0;
				visited.push_back(auxNode);
				i++;
			}
		}
	}

	auxNode = nullptr;

	return visited.size() == data.iterations + 1;
}

bool IsometricMapGenerator::GenerateWalls()
{
	BROFILER_CATEGORY("IsometricMapGeneration -- Walls Generation", Profiler::Color::Azure);

	LOG("Generating Walls...");
	
	for (uint i = 0u; i < visited.size(); ++i)
	{
		if (SDL_RectEquals(&visited[i]->whatToBlit, &SDL_Rect(FLOOR)))
		{
			IsometricMapNode* auxNode = visited[i];

			if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit, &SDL_Rect(WATER)))
			{
				nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit = WALL;
			}

			if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit, &SDL_Rect(WATER)))
			{
				nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit = WALL;
			}

			if (SDL_RectEquals(&nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(WATER)))
			{
				nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit = WALL;
			}

			if (SDL_RectEquals(&nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(WATER)))
			{
				nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit = WALL;
			}
		}
	}

	return true;
}

void IsometricMapGenerator::UpdateNode(IsometricMapNode* nodetocheck, int type)
{
	// For every kind of node do you have change it is data. Ex:
	if (type == type == (int)nodeType::typeTiledWall)
		nodetocheck->whatToBlit = WALL;
	else if (type == type == (int)nodeType::typeWall)
		nodetocheck->whatToBlit = WALL;
}

int IsometricMapGenerator::CheckTypeOfNode(IsometricMapNode* nodetocheck)
{
	BROFILER_CATEGORY("IsometricMapGeneration -- CheckTypeOfNode", Profiler::Color::Azure);

	bool up, down, left, right;
	up = down = left = right = false;

	if (CheckBoundaries({ nodetocheck->pos.x,nodetocheck->pos.y + 1 }))
	{
		if (SDL_RectEquals(&nodes[Get(nodetocheck->pos.x, nodetocheck->pos.y + 1)]->whatToBlit, &SDL_Rect(FLOOR)))
		{
			down = true;
		}
	}

	if (CheckBoundaries({ nodetocheck->pos.x, nodetocheck->pos.y - 1 }))
	{
		if (SDL_RectEquals(&nodes[Get(nodetocheck->pos.x, nodetocheck->pos.y - 1)]->whatToBlit, &SDL_Rect(FLOOR)))
		{
			up = true;
		}
	}

	if (CheckBoundaries({ nodetocheck->pos.x + 1, nodetocheck->pos.y }))
	{
		if (SDL_RectEquals(&nodes[Get(nodetocheck->pos.x + 1, nodetocheck->pos.y)]->whatToBlit, &SDL_Rect(FLOOR)))
		{
			left = true;
		}
	}

	if (CheckBoundaries({ nodetocheck->pos.x - 1, nodetocheck->pos.y }))
	{
		if (SDL_RectEquals(&nodes[Get(nodetocheck->pos.x - 1, nodetocheck->pos.y)]->whatToBlit, &SDL_Rect(FLOOR)))
		{
			right = true;
		}
	}

	// Create a new return for every kind of node do you want to check. Ex:
	if (up && right && left && down)
		return (int)nodeType::typeFully;
	else
		return (int)nodeType::noType;
}

void IsometricMapGenerator::getSize(uint& w, uint& h) const
{
	w = sizeX;
	h = sizeY;
}