#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Player.h"
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

	void EnemyMove(float dt, int enemyAreaLimitL, int enemyAreaLimitR);
	bool Rd();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnEndCollision(PhysBody* physA, PhysBody* physB);

	void reLoadXML(pugi::xml_node& parameters);
	// L07 DONE 6: Define OnCollision function for the player. 
	/*void OnCollision(PhysBody* physA, PhysBody* physB);*/

public:

	Player* player=nullptr;


	bool isPicked = false;

	Animation SPosition;
	SDL_Rect* spritePositions;

	Animation* currentAnim1 = nullptr;
	Animation idleAnim1;
	Animation downAnim1;
	Animation upAnim1;
	Animation rightAnim1;
	Animation leftAnim1;

	float rangeSize = 700.0f;
	float leftTopX;
	float leftTopY;
	float rightBottomX;
	float rightBottomY;

	float atk_rangeSize = 40.0f;
	float atk_leftTopX;
	float atk_leftTopY;
	float atk_rightBottomX;
	float atk_rightBottomY;

	bool AtackPlayer = false;


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
	bool showpath = true;
	bool player_range = false;

	SDL_Texture* mouseTileTex = nullptr;
	const char* texturePath;
	bool originSelected = false;
	iPoint origin;

	iPoint destination;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	/*PhysBody* pbody;*/
private:



	SDL_Texture* Enemytexture;
	const char* EnemyPath;
	PhysBody* pbody;
};

#endif // __ENEMY_H__