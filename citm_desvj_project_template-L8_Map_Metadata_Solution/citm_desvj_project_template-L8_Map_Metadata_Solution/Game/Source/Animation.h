#ifndef __ANIMATION_H__
#define __ANIMATION_H__




#include "SDL/include/SDL_rect.h"
#include "Entity.h"

#define MAX_FRAMES 2500

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	int animeFrameCount = 0;
	int aniFrameTotal = 0;
	const char* aniName;

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;



public:


	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
		loopCount = 0;
	}

	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	SDL_Rect& GetCurrentFrame()
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	SDL_Rect* SpritesPos(int totalPos, int weight, int hight, int ancho)
	{

		SDL_Rect* anirec = new SDL_Rect[totalPos];
		int x = 0;
		int y = 0;
		for (int i = 0; i < totalPos; i++)
		{
			anirec[i].x = x;
			anirec[i].y = y;
			anirec[i].w = weight;
			anirec[i].h = hight;
			if (x == ancho - weight) {
				x = 0;
				y += hight;
			}
			else {
				x += weight;

			}
		}
		return anirec;
	}

	void LoadAnim(char* Anipart, const char* NombreAni, SDL_Rect* spritePositions) {


		aniName = NombreAni;
		pugi::xml_document configFile;
		pugi::xml_node AniInfo;
		pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
		AniInfo = configFile.child("config").child("Animations").child(Anipart).child(NombreAni);

		//printf("%c", AniInfo);
		if (AniInfo.attribute("start").as_int() > AniInfo.attribute("end").as_int()) {
			for (int i = AniInfo.attribute("start").as_int(); i > AniInfo.attribute("end").as_int(); i--)
			{
				this->PushBack({ spritePositions[i] });

			}
		}
		else {
			for (int i = AniInfo.attribute("start").as_int(); i < AniInfo.attribute("end").as_int(); i++)
			{
				this->PushBack({ spritePositions[i] });

			}
		}
		this->speed = AniInfo.attribute("speed").as_float();
		this->loop = AniInfo.attribute("loop").as_bool();
	}

	const char* getNameAnimation() {

		return aniName;
	}

};


#endif
