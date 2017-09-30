#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"


// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;

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

	// TODO 1: Create methods to save and load
	// that can be called anytime, even if they 
	// will one execute by the very end of the frame
	// Load / Save
	void Trigger_Load() 
	{
		trigger_load_module = true;
	};

	void Trigger_Save() 
	{
		trigger_save_module = true;
	};

private:

	// Load config file
	// Don't load in awake, that not organized
	bool LoadConfig();

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

	// Call for the save/load modules if triggered
	bool Load();

	const bool Save();

public:

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1Map*				map;


private:

	p2List<j1Module*>	modules;
	uint				frames;
	float				dt;

	// TODO 1.2: Create two new variables from pugui namespace:
	// a xml_document to store the while config file and
	// a xml_node to read specific branches of the xml
private:
	pugi::xml_document	document;		//TODO 1.2.2 Doc
	pugi::xml_node		root_node;		//TODO 1.2.3 Node
	pugi::xml_node		app_config;		//TODO 1.2.4 To get access to jsut app specific config


private:
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	bool				trigger_load_module;
	bool				trigger_save_module;
};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif