#include "EntityManager.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

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
		pugi::xml_node root = sprites_doc.child("sprites");

		pugi::xml_node entity_n = config.child("entity");

		while (entity_n != NULL) {
			AddEntity(entity_n.attribute("type").as_uint());
			entity_n = entity_n.next_sibling("entity");
		}

		p2List_item<Entity*>* item = entities.start;

		pugi::xml_node temp = root.child("entity");

		while (item != NULL && ret == true) {
			ret = item->data->Awake(config.child(item->data->name.GetString()), temp);
			item = item->next;
			temp = temp.next_sibling("entity");
		}
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

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->CleanUp();
		item = item->next;
	}

	entities.clear();

	return ret;
}

bool EntityManager::PreUpdate(float dt) // Only for movement collisions
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->PreUpdate(dt);

		item->data->collision_box->rect.w = item->data->current_animation->GetCurrentFrame().w;
		item->data->collision_box->rect.h = item->data->current_animation->GetCurrentFrame().h;

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
bool EntityManager::PostUpdate(float dt) // // Only for movement collisions
{
	bool ret = true;

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true) {
		ret = item->data->PostUpdate(dt);
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

void EntityManager::Draw(float dt) {

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL) {
		item->data->Draw(dt);
		item = item->next;
	}

}