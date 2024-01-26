#include "SceneLogo.h"
#include "Scene.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Window.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"
#include <iostream>
#include <stdio.h>

//#include "SceneEnding.h"
//
//#include "Optick/include/optick.h"
//
//#include "GuiButton.h"
//#include "Window.h"
//#include "GuiManager.h"



SceneLogo::SceneLogo() : Module() {
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo() {

}

bool SceneLogo::Awake(pugi::xml_node& config) {

	bool ret = true;

	//textureLogoPath = config.child("textureLogoPath").attribute("path").as_string();
	
	alpha = 0.0f;
	toFade = false;
	prueba = false;
	toFadeButton = false;

	return ret;
}

bool SceneLogo::Start() {
	
	LOG("Loading background assets - SceneLogo");

	bool ret = true;

	/*textureLogo = app->tex->Load(textureLogoPath.GetString());*/
	textureLogo = app->tex->Load("Assets/Textures/Intro.png");
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->render->camera.w = 0;
	app->render->camera.h = 0;
	
	/*uint w, h;*/
	
	//app->win->GetWindowSize(w, h);
	//button1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, " Next ", { (int)w / 2 - 50,640,
	//	app->win->buttonW,app->win->buttonH }, this);
	//button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Button 2", { (int)w / 2 - 50,(int)h / 2,
	//	app->win->buttonW,app->win->buttonH }, this);




	return true;
}

bool SceneLogo::Update(float dt)
{


	if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		prueba = true;

	}


	//if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || toFadeButton)
	//{
	//	toFadeButton = false;

	//	app->fade->Fade(this, app->scene, 100);

	//}

	//if (button1->state == GuiControlState::NONE)
	//{
	//	std::cout << "NONE" << std::endl;
	//}
	
	

	return true;
}


bool SceneLogo::PostUpdate()
{

	SDL_Rect rect = { 0,0,1280,768 };

	app->render->DrawTexture2(textureLogo, 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647,false);
	app->render->DrawRectangle(rect, 0, 0, 0, (unsigned char)(255.0f * alpha));
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		LOG("ENTER");
		app->fade->Fade(app->sceneLogo, app->scene, 500);

	}
		



	//app->guiManager->Draw();
	



	return true;
}

bool SceneLogo::CleanUp()
{
	//memoryleak
	app->tex->UnLoad(textureLogo);

	return true;
}


//bool SceneLogo::OnGuiMouseClickEvent(GuiControl* control)
//{
//	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
//	LOG("Event by %d ", control->id);
//
//	switch (control->id)
//	{
//	case 1:
//		LOG("Button 1 click");
//		toFadeButton = true;
//		button1->state = GuiControlState::NONE;
//		
//		break;
//
//	}
//
//	return true;
//}
