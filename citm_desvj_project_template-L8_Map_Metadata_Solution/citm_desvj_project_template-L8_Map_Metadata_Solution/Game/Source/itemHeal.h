#pragma once
#ifndef __ITEMHEAL_H__
#define __ITEMHEAL_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class itemHeal : public Entity
{
public:

	itemHeal();
	virtual ~itemHeal();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	

public:

	bool isPicked = false;

	Animation dd;
	Animation* currentAnimation = nullptr;

	int vida = 4;

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;

	//L07 DONE 4: Add a physics to an item
	PhysBody* pbody;
};

#endif // __ITEM_H__