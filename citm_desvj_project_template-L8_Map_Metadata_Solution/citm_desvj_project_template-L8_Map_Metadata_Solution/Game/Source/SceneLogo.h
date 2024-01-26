#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__
#define NUM_IMAGES 150

#include "Module.h"
#include "Animation.h"
#include <iostream>

struct SDL_Texture;

class SceneLogo : public Module
{
public:

	SceneLogo();

	// Destructor
	virtual ~SceneLogo();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:

	SDL_Texture* textureLogo;
	int frame;
	float alpha;
	Animation sceneLogo;
	SString textureLogoPath;

};



#endif // !__SCENE_LOGO_H__



