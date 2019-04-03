#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

#include "RoomsMapGenerator.h"
#include "IsometricMapGenerator.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	IsometricMapData mapInfo;
	MapInfo mapData;
};

#endif // __j1SCENE_H__