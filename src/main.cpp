#include <SDL.h>
#include <SDL_ttf.h>

#include "StateMachine.h"
#include "Game.h"
#include "MainMenu.h"

int main(int argc, char** args) {

	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
	assert(TTF_Init() == 0);

	StateMachine states("The Great Escape", 750, 650);
	states.Push<MainMenu>(750, 650);
	states.Push<Game>(750, 650);
	states.Run();

	return 0;
}