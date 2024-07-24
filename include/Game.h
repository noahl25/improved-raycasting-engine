#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Raycaster.h"
#include "UI.h"

class Game
{
public:

	Game(int width, int height);
	~Game();

	void Run();

private:

	void HandleEvents();
	void Render();

private:

	Renderer m_Renderer;

	SDL_Window* m_Window;
	Raycaster m_Raycaster;
	World m_World;
	Camera m_Camera;

	UI m_UI;

	UIGroup m_Popup;

	SDL_Event m_Event{};

	int m_Width, m_Height;
	bool m_Running = true;
	bool m_Paused = false;
	float m_DeltaTime = 0.016f;

};
