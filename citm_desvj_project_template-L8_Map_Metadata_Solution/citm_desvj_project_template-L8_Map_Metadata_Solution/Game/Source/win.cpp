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
#include "win.h"


win::win() : Entity(EntityType::WIN)
{
	name.Create("win");
}

win::~win() {}

bool win::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	return true;
}

bool win::Start() {

	////initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	/*app->tex->GetSize(texture, texW, texH);*/
	pbody = app->physics->CreateCircle(position.x , position.y , 8, bodyType::STATIC);

	//// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::WIN;
	pbody->listener = this;
	touch1 = 0;
	pickCoinFxId = app->audio->LoadFx(parameters.attribute("winpath").as_string());
	return true;
}

bool win::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  


	SDL_Rect rect;
	rect = { 10,10,512,512 };

	

	

	/*for (int i = 0; i < vida; i++)
	{
		app->render->DrawTexture(texture, 10 + i * 20, 1, 0.1, SDL_FLIP_NONE, &rect, 0, 0);
	}*/

	//if (touch1 = true) {
	//	/*texture = app->tex->Load(texturePath);
	//	app->render->DrawTexture(texture, 150, 100, 0.5, SDL_FLIP_NONE, &rect, 0, 0);*/
	//	/*app->audio->PlayFx(pickCoinFxId);*/
	//}
	for (int i = 0; i < touch1; i++)
	{
		app->render->DrawTexture(texture, 150 + i * 20, 1, 1, SDL_FLIP_NONE, &rect, 0, 0);
	}

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
	return true;
}

bool win::CleanUp()
{
	return true;
}
void win::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		/*app->scene->GetVida()->vida++;*/
		/*touch1 = true;*/
		app->audio->PlayFx(pickCoinFxId);
		/*SDL_Rect rect;
		rect = { 10,10,512,512 };
		texture = app->tex->Load(texturePath);
		app->render->DrawTexture(texture, 150, 100, 0.5, SDL_FLIP_NONE, &rect, 0, 0);*/
		touch1++;
		LOG("Collision touch1");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}