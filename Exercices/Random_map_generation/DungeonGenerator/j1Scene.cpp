#include "Defs.h"
#include "Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"

#include <string>

#define FLOOR { 388,249,32,32 }

j1Scene::j1Scene() : j1Module()
{
	name = "scene";
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	pugi::xml_document	map_file;
	pugi::xml_node		map;

	map_file.load_file("mapsData.xml");
	map = map_file.child("MapsData").child("maps");

	mapInfo.sizeX = map.child("IsometricMap").child("gridSize").attribute("xSize").as_int();
	mapInfo.sizeY = map.child("IsometricMap").child("gridSize").attribute("ySize").as_int();
	mapInfo.tileSize = map.child("IsometricMap").child("tileSize").attribute("size").as_uint();
	mapInfo.iterations = map.child("IsometricMap").child("mapIterations").attribute("iterations").as_uint();
	mapInfo.tilesetPath = "tilesets/isometricMap.png";

	App->isometricMap->GenerateMap(mapInfo);

	//TODO 1: Load all "RoomsMap" data from mapsData.xml. Then the code will compile correctly. Use RoomsMapInfo mapData.
	

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 20;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 20;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 20;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 20;


	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		App->isometricMap->CleanUp();
		App->roomsMap->CleanDungeon();

		App->roomsMap->GenerateDungeon(mapData);
	}

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		App->isometricMap->CleanUp();
		App->roomsMap->CleanDungeon();

		App->isometricMap->GenerateMap(mapInfo);
	}

	App->roomsMap->Draw();
	App->isometricMap->DrawMap();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
