#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "itemHeal.h"


itemHeal::itemHeal() : Entity(EntityType::ITEMHEAL)
{
	name.Create("itemheal");
}

itemHeal::~itemHeal() {}

bool itemHeal::Awake() {

	texturePath = parameters.attribute("texturepath").as_string();
	return true;
}

bool itemHeal::Start() {

	////initilize textures
	texture = app->tex->Load(texturePath);

	//// L07 DONE 4: Add a physics to an item - initialize the physics body
	//app->tex->GetSize(texture, texW, texH);
	//pbody = app->physics->CreateCircle(position.x + texH / 2, position.y + texH / 2, 8, bodyType::DYNAMIC);

	//// L07 DONE 7: Assign collider type
	//pbody->ctype = ColliderType::ITEM;

	return true;
}

bool itemHeal::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	SDL_Rect rect;
	rect = {10,10,512,512};

	for (int i = 0; i < vida; i++)
	{
		app->render->DrawTexture(texture, 10 + i * 20, 1 , 0.1, SDL_FLIP_NONE, &rect, 0, 0);
	}
	


	return true;
}

bool itemHeal::CleanUp()
{
	return true;
}