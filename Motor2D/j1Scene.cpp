#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "ent_Player.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Scene.h"

#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scenes");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");

	return true;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	ret = App->map->Load("Scene2.tmx");

	CreateEntities();

	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;

	App->render->camera = App->render->CameraInitPos();

	return ret;
}

void j1Scene::CreateEntities()
{
	p2List_item<ColliderObject*>* position = App->map->data.colliders.start; //iterate all objects of tile to find entities
	j1Entity* ent = nullptr;

	for (; position; position = position->next) {
		if (position->data->name == "Player") {
			ent = App->entities->CreateEntity(j1Entity::Types::PLAYER, position->data->coll_x, position->data->coll_y);
		}
		else ent = nullptr;

		if (ent != nullptr) {
			ent->data.tileset.texture = App->tex->Load(ent->data.tileset.imagePath.GetString());
		}
	}
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("UpdateScene", Profiler::Color::Red);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 300 * dt;

	App->map->Draw();	
	
	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return App->entities->CleanUp();
}