#include "Game.h"

int main(int argc, char** args) {

	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);

	int w = display.w / 2.0f;
	int h = display.h / 1.5f;

	Game game(w, h);
	game.Run();

	return 0;
}