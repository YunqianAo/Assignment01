#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	name.Create("players");
	idleAnim1.PushBack({ 0, 0, 16,15 });
	idleAnim1.PushBack({ 16, 0, 16,15 });
	idleAnim1.PushBack({ 16*2, 0, 16,15 });
	idleAnim1.PushBack({ 16 * 3, 0, 16,15 });
	idleAnim1.PushBack({ 16 * 4, 0, 16,15 });
	idleAnim1.PushBack({ 16 * 5, 0, 16,15 });
	idleAnim1.loop = true;
	idleAnim1.speed = 0.1f;

	leftAnim1.PushBack({ 0, 0, 16,15 });
	leftAnim1.PushBack({ 16, 0, 16,15 });
	leftAnim1.PushBack({ 16 * 2, 0, 16,15 });
	leftAnim1.loop = true;
	leftAnim1.speed = 0.1f;

	rightAnim1.PushBack({ 0, 0, 16,15 });
	rightAnim1.PushBack({ 16, 0, 16,15 });
	rightAnim1.PushBack({ 16 * 2, 0, 16,15 });
	rightAnim1.loop = true;
	rightAnim1.speed = 0.1f;

	upAnim1.PushBack({ 0, 0, 16,15 });
	
	upAnim1.loop = true;
	upAnim1.speed = 0.1f;

	downAnim1.PushBack({ 16, 0, 16,15 });
	downAnim1.PushBack({ 16*3, 0, 16,15 });
	downAnim1.PushBack({ 16 * 5-2, 0, 16,15 });
	downAnim1.PushBack({ 100, 0, 16,15 });
	downAnim1.PushBack({ 131, 0, 16,15 });
	downAnim1.PushBack({ 162, 0, 16,15 });
	downAnim1.loop = true;
	downAnim1.speed = 0.1f;

	//in constructor
	
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
	
	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath1").as_string());
	
	currentAnim1 = &idleAnim1;

	remainingJumpSteps = 0;
	death = 0;
	// L07 DONE 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, 8, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	jumpForce = 0;
	return true;
}

bool Player::Update(float dt)
{
	texture = app->tex->Load(config.attribute("texturePath1").as_string());
	currentAnim1 = &idleAnim1;
	// L07 DONE 5: Add physics to the player - updated player position using physics

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
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
		texture = app->tex->Load(config.attribute("texturePath4").as_string());
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
		
		
	}


	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
	/*app->render->DrawTexture(texture,position.x,position.y);*/
	app->render->camera.x = -position.x*3+200;
	app->render->camera.y = -450;
	LOG("%d,%d", position.x, position.y);

	//Intento de hacer que la camara cambie a cierta altura
	//if (position.y >= 379)
	//{
	//	app->render->camera.y = -600;
	//}
	//
	currentAnim1->Update();
	return true;
}

bool Player::CleanUp()
{
	return true;
}

bool Player::PostUpdate() {
	SDL_Rect rect = currentAnim1->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x , position.y , &rect);

	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
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