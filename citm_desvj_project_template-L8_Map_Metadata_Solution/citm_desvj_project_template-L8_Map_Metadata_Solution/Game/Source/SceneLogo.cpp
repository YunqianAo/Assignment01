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



SceneLogo::SceneLogo() : Module() {
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo() {

}

bool SceneLogo::Awake(pugi::xml_node& config) {

	bool ret = true;
	
	alpha = 0.0f;


	return ret;
}

bool SceneLogo::Start() {
	
	LOG("Loading background assets - SceneLogo");

	bool ret = true;

	//textureLogo = app->tex->Load(textureLogoPath.GetString())//No funciona por algun motivo
	textureLogo = app->tex->Load("Assets/Textures/Intro.png");
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->render->camera.w = 0;
	app->render->camera.h = 0;
	
	return true;
}

bool SceneLogo::Update(float dt)
{
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

	return true;
}

bool SceneLogo::CleanUp()
{
	
	app->tex->UnLoad(textureLogo);

	return true;
}


