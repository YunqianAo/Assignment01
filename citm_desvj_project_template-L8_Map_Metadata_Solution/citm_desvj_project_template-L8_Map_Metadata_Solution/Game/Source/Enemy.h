#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();
	
	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation* currentAnim1 = nullptr;
	Animation idleAnim1;
	Animation downAnim1;
	Animation upAnim1;
	Animation rightAnim1;
	Animation leftAnim1;
	//class member variable
	int remainingJumpSteps;

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	//Audio fx
	int pickCoinFxId;

	

	bool jumpTimer;
	int framesJump;
	int jumpForce;
	int death;


	const char* texturePath;

	iPoint destination;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
};

#endif // __ENEMY_H__