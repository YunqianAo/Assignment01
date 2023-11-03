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
	idleAnim1.PushBack({ 1, 0, 14,17 });
	idleAnim1.loop = true;
	idleAnim1.speed = 0.001f;

	leftAnim1.PushBack({ 1, 36, 14, 17 });
	leftAnim1.PushBack({ 17,36, 14, 17 });
	leftAnim1.PushBack({ 33, 36, 14, 17 });
	leftAnim1.loop = true;
	leftAnim1.speed = 0.1f;

	rightAnim1.PushBack({ 2, 54, 14, 17 });
	rightAnim1.PushBack({ 18, 54, 14, 17 });
	rightAnim1.PushBack({ 34, 54, 14, 17 });
	rightAnim1.loop = true;
	rightAnim1.speed = 0.1f;

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

	texture = app->tex->Load(config.attribute("texturePath").as_string());
	
	currentAnim1 = &idleAnim1;

	remainingJumpSteps = 0;
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

	currentAnim1 = &idleAnim1;
	// L07 DONE 5: Add physics to the player - updated player position using physics

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		currentAnim1 = &leftAnim1;
		velocity.x = -0.2*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		currentAnim1 = &rightAnim1;
		velocity.x = 0.2*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		remainingJumpSteps = 20;
		jumpForce = 100;
		
	}
	if (remainingJumpSteps > 0) {
		pbody->body->ApplyForce(b2Vec2(0, -jumpForce), pbody->body->GetWorldCenter(), true);
		jumpForce = jumpForce - 5;
		remainingJumpSteps--;
	}

	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

	/*app->render->DrawTexture(texture,position.x,position.y);*/

	
	currentAnim1->Update();
	return true;
}

bool Player::CleanUp()
{
	return true;
}

bool Player::PostUpdate() {
	SDL_Rect rect = currentAnim1->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x +28, position.y +25, &rect);
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
		break;
	default:
		break;
	}
}