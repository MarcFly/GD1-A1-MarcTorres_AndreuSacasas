#include "EntityManager.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Pathfinding.h"

EntityManager::EntityManager()
{
	name.create("entities");
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(const pugi::xml_node& config)
{
	bool ret = true;

	tick_cap = config.child("ticks").attribute("value").as_uint();

	pugi::xml_parse_result result = sprites_doc.load(config.child("spritesheet").attribute("source").as_string());

	if (result == NULL) { //Check that it loaded
		LOG("Could not load sprites xml file sprites.xml. pugi error: %s", result.description());
		//ret = false;
	}
	
		pugi::xml_node root = sprites_doc.child("sprites");
		
		pugi::xml_node entity_n = config.child("entity");

		while (entity_n != NULL) {
			AddEntity(entity_n.attribute("type").as_uint());
			entity_n = entity_n.next_sibling("entity");
		}

		p2List_item<Entity*>* item = entities.start;

		while (item != NULL && ret == true) {
			ret = item->data->Awake(config.child(item->data->name.GetString()), root.child(item->data->name.GetString()));
			item = item->next;
		}

	
	return ret;
}

int EntityManager::AddEntity(const uint& name)
{
	if (name == (uint)player) {
		entities.add(new j1Player());
		return (int)player;
	}

	
	return none;
}

bool EntityManager::Start()
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

bool EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->CleanUp();
		item = item->next;
	}

	entities.clear();

	return ret;
}

bool EntityManager::PreUpdate()
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->PreUpdate();
		item = item->next;
	}

	return ret;
}

bool EntityManager::UpdateTick(float dt) 
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->UpdateTick(dt);
		item = item->next;
	}

	return ret;
}

bool EntityManager::Update(float dt)
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->Update(dt);
		item = item->next;
	}

	return ret;
}
bool EntityManager::PostUpdate()
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->PostUpdate();
		item = item->next;
	}

	return ret;
}

bool EntityManager::Load(const pugi::xml_node& savegame)
{
	bool ret = true;
	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->Load(savegame);
		item = item->next;
	}


	return ret;
}

bool EntityManager::Save(pugi::xml_node& savegame)
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->Save(savegame);
		item = item->next;
	}

	return ret;
}