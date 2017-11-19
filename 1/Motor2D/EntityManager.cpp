#include "EntityManager.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Crawler.h"
#include "Flyer.h"

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
	tex_folder.create(config.child("folder").attribute("source").as_string());

	p2SString load_s;
	load_s.create("%s%s", tex_folder.GetString(), config.child("sprites").attribute("source").as_string());
	pugi::xml_parse_result result = sprites_doc.load_file(load_s.GetString());

	if (result == NULL) { //Check that it loaded
		LOG("Could not load sprites xml file sprites.xml. pugi error: %s", result.description());
		ret = false;
	}
	else {
		
		LoadEntities();
	}
	
	return ret;
}

int EntityManager::AddEntity(const uint& name, const uint& eid)
{
	if (name == (uint)player) {
		entities.add(new j1Player(name, eid));
		return (int)player;
	}
	else if (name == (uint)crawler)
	{
		entities.add(new Crawler(name, eid));
		return (int)crawler;
	}
	else if (name == (uint)flyer) {
		entities.add(new Flyer(name, eid));
		return (int)flyer;
	}
	
	return none;
}

bool EntityManager::Start()
{
	bool ret = true;

	p2List_item<tex>* item_tex = texs.start;

	while (item_tex != NULL && ret == true) {
		item_tex->data.texture = App->tex->Load(item_tex->data.source.GetString());
		
		if (item_tex->data.texture == NULL) {
			ret = false;
			LOG("Error Loading entity textures.");
		}
		item_tex = item_tex->next;
	}

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

	CleanEntities();
	
	p2List_item<tex>* item = texs.start;

	while (item != NULL)
	{
		App->tex->UnLoad(item->data.texture);
		item = item->next;
	}

	texs.clear();

	return ret;
}

bool EntityManager::PreUpdate(float dt) // Only for movement collisions
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true && item->data != nullptr) {
		ret = item->data->PreUpdate(dt);

		item = item->next;
	}

	return ret;
}

bool EntityManager::UpdateTick(float dt) 
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true && item->data != nullptr) {
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
		if(item != nullptr && item->data != nullptr && item->data != NULL)
			ret = item->data->Update(dt);
		item = item->next;
	}

	return ret;
}
bool EntityManager::PostUpdate(float dt) // // Only for movement collisions
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true && item->data != nullptr) {
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	return ret;
}

bool EntityManager::Load(const pugi::xml_node& savegame)
{
	bool ret = true;

	CleanEntities();
	
	p2List_item<Entity*>* item;

	pugi::xml_node temp = savegame.first_child();

	while (temp != NULL && ret == true) {
		
		AddEntity(temp.attribute("type").as_uint(), FindEntities(temp.attribute("type").as_uint()));

		item = entities.end;
		
		//while (temp.attribute("type").as_int() != item->data->type && temp.attribute("entity_id").as_int() != item->data->entity_id)
		
		ret = item->data->Load(temp);

		temp = temp.next_sibling("entity");
	}


	return ret;
}

bool EntityManager::Save(pugi::xml_node& savegame)
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		if(item->data != nullptr)
			ret = item->data->Save(savegame);
		item = item->next;
	}

	return ret;
}

void EntityManager::Draw(float dt) {

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL) {
		if(item != nullptr && item->data != nullptr)
			item->data->Draw(dt);
		item = item->next;
	}

}

int EntityManager::FindEntities(const uint& type, const uint& eid) {


	int ret = 0;
	if (eid == INT_MAX)
	{
		p2List_item<Entity*>* item = entities.start;

		while (item != NULL)
		{
			if (item->data->type == type)
				ret++;

			item = item->next;
		}
	}
	else
	{
		for (int i = 0; i < entities.count(); i++)
		{
			if (entities.At(i)->data != nullptr && entities.At(i)->data->type == type && entities.At(i)->data->entity_id == eid) {
				ret = i;
				break;
			}
		}
	}

	return ret;
}

bool EntityManager::CleanEntities() {
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL)
	{
		if (item->data != nullptr) {
			ret = item->data->CleanUp();
			//ret = item->data->SpecificCleanUp();
		}
		item = item->next;
	}

	entities.clear();

	return ret;
}

bool EntityManager::DestroyEntity(const int& at) {
	bool ret = true;
	
	if(entities.At(at)->data != nullptr)
		ret = entities.At(at)->data->CleanUp();
	entities.At(at)->data = nullptr;

	return ret;
}

bool EntityManager::LoadEntities()
{
	bool ret = true;

	pugi::xml_node root = sprites_doc.child("sprites");

	p2List_item<Entity*>* item;

	p2SString curr_map("%i", App->scene->curr_map);
	pugi::xml_node temp_properties = root.find_child_by_attribute("value", curr_map.GetString()).child("entity");

	while (temp_properties.attribute("type").as_string() != "" && ret == true) {
		AddEntity(temp_properties.attribute("type").as_uint(), FindEntities(temp_properties.attribute("type").as_uint()));

		item = entities.end;

		pugi::xml_node temp_sprites = root.child("animations");
		while (temp_sprites.attribute("type").as_int() != item->data->type)
			temp_sprites = temp_sprites.next_sibling();

		ret = item->data->Awake(temp_sprites, temp_properties);
		ret = item->data->Start();
		item = item->next;
		temp_properties = temp_properties.next_sibling("entity");
	}

	return ret;
}

Entity* EntityManager::FindByColl(const Collider* c)
{
	p2List_item<Entity*>* item = entities.start;

	while (item != NULL)
	{
		if (item->data != nullptr && c == item->data->collision_box)
			return item->data;

		item = item->next;
	}

	return nullptr;
}
