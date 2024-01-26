#pragma once
#ifndef __WIN_H__
#define __WIN_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"


struct SDL_Texture;

class win : public Entity
{
public:

	win();
	virtual ~win();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:
	int touch1 = 0;
	bool isPicked = false;

	Animation dd;
	Animation* currentAnimation = nullptr;

	int vida = 4;
	int pickCoinFxId;


private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;

	//L07 DONE 4: Add a physics to an item
	PhysBody* pbody;
};



#endif // __WIN_H__