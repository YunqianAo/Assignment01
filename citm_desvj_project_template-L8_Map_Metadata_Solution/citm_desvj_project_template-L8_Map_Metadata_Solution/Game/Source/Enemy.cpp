#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"
#include "Player.h"
#include "Box2D/Box2D/Dynamics/b2Body.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("enemy");
	idleAnim1.PushBack({ 1, 0, 8,11 });
	idleAnim1.PushBack({ 13, 0, 8,11 });
	idleAnim1.PushBack({ 25, 0, 8,11 });
	idleAnim1.PushBack({ 27, 0, 8,11 });
	
	idleAnim1.loop = true;
	idleAnim1.speed = 0.1f;

	
	//in constructor
	
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(parameters.attribute("x1").as_int(), config.attribute("y1").as_int());
	texture = app->tex->Load(parameters.attribute("texturePath").as_string());


	currentAnim1 = &idleAnim1;

	destination = app->scene->player->position;
	return true;
}

bool Enemy::Start() {

	texture = app->tex->Load(parameters.attribute("texturePath").as_string());

	remainingJumpSteps = 0;
	death = 0;
	// L07 DONE 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x , position.y , 5, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	jumpForce = 0;

	b2Filter enemyFilter;
	enemyFilter.categoryBits = static_cast<uint16_t>(ColliderType::PLATFORM);
	enemyFilter.maskBits = 0xFFFF & ~static_cast<uint16_t>(ColliderType::PLATFORM);
	pbody->body->GetFixtureList()[0].SetFilterData(enemyFilter);

	player = app->scene->getPlayer();
	return true;
}

bool Enemy::Update(float dt)
{
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);


	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

	SDL_Rect rect = currentAnim1->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	currentAnim1->Update();
	return true;
}

bool Enemy::CleanUp()
{
	if (pbody != nullptr) {
		app->physics->GetWorld()->DestroyBody(pbody->body);
	}

	if (Enemytexture) {
		SDL_DestroyTexture(Enemytexture);
		Enemytexture = nullptr;
	}
	return true;
}

bool Enemy::PostUpdate() {
	SDL_Rect rect = currentAnim1->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x , position.y , &rect);

	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		app->scene->GetVida()->vida--;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");	
		texture = app->tex->Load(config.attribute("texturePath5").as_string());
		currentAnim1 = &downAnim1;
		break;
	default:
		break;
	}
}