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

	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.Awake(config.child(item->data.name.GetString()));
		item = item->next;
	}

	return ret;
}

bool EntityManager::Start()
{
	bool ret = true;

	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.Start();
		item = item->next;
	}

	return ret;
}

bool EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.CleanUp();
		item = item->next;
	}

	return ret;
}

bool EntityManager::PreUpdate()
{
	bool ret = true;

	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.PreUpdate();
		item = item->next;
	}

	return ret;
}

bool EntityManager::UpdateTick(float dt) 
{
	bool ret = true;

	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.UpdateTick(dt);
		item = item->next;
	}

	return ret;
}

bool EntityManager::Update(float dt)
{
	bool ret = true;

	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.Update(dt);
		item = item->next;
	}

	return ret;
}
bool EntityManager::PostUpdate()
{
	bool ret = true;

	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.PostUpdate();
		item = item->next;
	}

	return ret;
}

bool EntityManager::Load(const pugi::xml_node& savegame)
{
	bool ret = true;
	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.Load(savegame);
		item = item->next;
	}


	return ret;
}

bool EntityManager::Save(pugi::xml_node& savegame)
{
	bool ret = true;

	p2List_item<Entity>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data.Save(savegame);
		item = item->next;
	}

	return ret;
}