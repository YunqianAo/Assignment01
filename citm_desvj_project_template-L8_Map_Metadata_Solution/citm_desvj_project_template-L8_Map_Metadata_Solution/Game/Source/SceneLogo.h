#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__
#define NUM_IMAGES 150

#include "Module.h"
#include "Animation.h"

//#include "SDL_Timer.h"
//#include "SceneTitle.h"
//#include "GuiCheckBox.h"
//#include "GuiSliderBar.h"
//

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

	//bool OnGuiMouseClickEvent(GuiControl* control);

public:
	// The scene sprite sheet loaded into an SDL_Texture
	//SDL_Texture* bgTexture[NUM_IMAGES];
	SDL_Texture* textureLogo;
	int frame;
	float alpha;
	bool toFade;

	bool prueba;

	bool toFadeButton;

	//Animation* currentAnimationSceneLogo = nullptr;
	
	Animation sceneLogo;

	

	//std::shared_ptr<Animation> cASL = std::make_shared<Animation>();

	//SDL_Timer chrono;
	SString textureLogoPath;
	
	//GuiButton* button1;

	//
	

};



#endif // !__SCENE_LOGO_H__



