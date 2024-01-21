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
	/*name.Create("Enemy");*/

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

	EnemyPath = parameters.attribute("texturepath").as_string();

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x1").as_int(), config.attribute("y1").as_int());
	destination = app->scene->player->position;
	return true;
}

bool Enemy::Start() {

	Enemytexture = app->tex->Load(EnemyPath);

	texture = app->tex->Load(config.attribute("texturePath1").as_string());
	
	currentAnim1 = &idleAnim1;

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
	enemyFilter.maskBits = 0xFFFF & ~static_cast<uint16_t>(ColliderType::PLATFORM);  // ÓëÈÎºÎÅö×²×é±ðµÄÎïÌå¶¼·¢ÉúÅö×²£¬µ«²»Óë×Ô¼º·¢ÉúÅö×²
	pbody->body->GetFixtureList()[0].SetFilterData(enemyFilter);

	player = app->scene->getPlayer();
	return true;
}

bool Enemy::Update(float dt)
{
	texture = app->tex->Load(config.attribute("texturePath4").as_string());
	currentAnim1 = &idleAnim1;

	
	// L07 DONE 5: Add physics to the player - updated player position using physics

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	destination.x = app->scene->getPlayer()->position.x;
	destination.y = app->scene->getPlayer()->position.y;
	int limitX = destination.x - position.x;
	int limitY = destination.y - position.y;
	if (limitX < 0) {
		limitX = -limitX;
	}
	if (limitY < 0) {
		limitY = -limitX;
	}
	if (limitX <= 100 && limitY <= 100) {
		app->map->pathfinding->CreatePath(position, destination);
		
	}
	if (app->map->pathfinding->GetLastPath()->Count() > 1) {
		iPoint newPositionPoint = app->map->MapToWorld(app->map->pathfinding->GetLastPath()->At(1)->x, app->map->pathfinding->GetLastPath()->At(1)->y);
		b2Vec2 newPosition = b2Vec2(PIXEL_TO_METERS(newPositionPoint.x), PIXEL_TO_METERS(newPositionPoint.y));
		pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y - GRAVITY_Y));
		if (position.x > newPositionPoint.x) {
			pbody->body->SetLinearVelocity(b2Vec2(-speed * dt, pbody->body->GetLinearVelocity().y - GRAVITY_Y));
		}
		else {
			pbody->body->SetLinearVelocity(b2Vec2(speed * dt, pbody->body->GetLinearVelocity().y - GRAVITY_Y));
		}
	}
	for (uint i = 0; i < app->map->pathfinding->GetLastPath()->Count(); ++i) {
		iPoint pos = app->map->MapToWorld(app->map->pathfinding->GetLastPath()->At(i)->x, app->map->pathfinding->GetLastPath()->At(i)->y);
		
	}
	
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	/*if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		texture = app->tex->Load(config.attribute("texturePath3").as_string());
		currentAnim1 = &leftAnim1;
		velocity.x = -0.2*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		texture = app->tex->Load(config.attribute("texturePath2").as_string());
		currentAnim1 = &rightAnim1;
		velocity.x = 0.2*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		texture = app->tex->Load(config.atyytribute("texturePath4").as_string());
		currentAnim1 = &upAnim1;
		remainingJumpSteps = 20;
		jumpForce = 100;
		
	}
	if (remainingJumpSteps > 0) {
		texture = app->tex->Load(config.attribute("texturePath4").as_string());
		currentAnim1 = &upAnim1;
		pbody->body->ApplyForce(b2Vec2(0, -jumpForce), pbody->body->GetWorldCenter(), true);
		jumpForce = jumpForce - 5;
		remainingJumpSteps--;
	}

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT) {
		death = 6;
		texture = app->tex->Load(config.attribute("texturePath5").as_string());
		currentAnim1 = &downAnim1;
		
		
	}*/
	/*if (death > 0) {
		texture = app->tex->Load(config.attribute("texturePath5").as_string());
		currentAnim1 = &downAnim1;
		death--;
	}*/

	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

	/*app->render->DrawTexture(texture,position.x,position.y);*/
	/*app->render->camera.x = -position.x*3+200;

	app->render->camera.y = -position.y*3+200;*/

	//PhysBody* c12 = app->physics->CreateRectangle(0 + 18 * 38 / 2, 19.5 * 18, 18 * 38, 18, STATIC);
	//c12->ctype = ColliderType::UNKNOWN;

	/*if (OnCollision(pbody,c12) {
		
	}*/
	
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