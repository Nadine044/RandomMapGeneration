#ifndef __j1APP_H__
#define __j1APP_H__

#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"

#include <list>
#include <string>

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Scene;
class j1PathFinding;
class RoomsMapGenerator;
class IsometricMapGenerator;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Scene*			scene = nullptr;
	RoomsMapGenerator*			roomsMap = nullptr;
	IsometricMapGenerator*		isometricMap = nullptr;
	j1PathFinding*		pathfinding = nullptr;

private:

	std::list<j1Module*> modules;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;
};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif