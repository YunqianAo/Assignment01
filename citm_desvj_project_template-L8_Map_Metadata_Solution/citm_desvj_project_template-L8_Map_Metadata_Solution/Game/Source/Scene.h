#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include"Enemy.h"
#include"itemHeal.h"
#include "win.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	Player* getPlayer();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	itemHeal* GetVida();
	win* GetWin();

public:
	Player* player;

	itemHeal* itemheal;
	win* winn;

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;

	//L03: DONE 3b: Declare a Player attribute
	
	Enemy* enemy;

};

#endif // __SCENE_H__