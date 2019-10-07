#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{
	const int fps = 60;
	const int frameDelay = 1000 / fps; //Max allowed time between frames

	Uint32 frameStart;

	int frameTime;

	game = new Game();

	while (game->Running())
	{
		frameStart = SDL_GetTicks();

		game->HandleEvents();
		game->Update();
		game->Render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->Clean();

	return 0;
}