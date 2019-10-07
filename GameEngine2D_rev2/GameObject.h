#pragma once
#include "Game.h"

class GameObject
{
public:
	GameObject(const char* textureSheet, int x, int y);
	~GameObject();

	
	///Function is called upon level load and should contain instructions for
	///Initializing the GameObject prior to rendering the first frame.
	void Awake();

	///Function is called after Awake and before rendering the first frame.
	///Instructions here should be used for passing information between
	///GameObjects after they have initialised.
	void Start();

	///Function is called once per frame
	void Update();

	///Function is called once per frame and is used to display the result of
	///all changes which have taken place on this frame.
	void Render();
	
private:
	int xPos;
	int yPos;

	std::string Tag;
	
	SDL_Texture* objTex;
	SDL_Rect srcRect, destRect;

};