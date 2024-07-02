#pragma once

#include <SDL.h>

#include "Raycaster.h"

class Game
{
public:

	Game(int width, int height);
	~Game();

	void Run();

private:

	void HandleEvents();
	void Update();

private:

	SDL_Window* m_Window;
	Raycaster m_Raycaster;
	World m_World;
	Camera m_Camera;

	SDL_Event m_Event{};

	int m_Width, m_Height;
	bool m_Running = true;
	bool m_Paused = false;

};
