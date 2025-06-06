#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Raycaster.h"
#include "UI.h"
#include "Animation.h"
#include "State.h"
#include "Audio.h"
#include "Random.h"

class Game : public State
{
public:

	Game(const Renderer& renderer, int width, int height);
	~Game();

private:

	void HandleEvents(float deltaTime) override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) override;

private:

	const Renderer& m_Renderer;

	Raycaster m_Raycaster;
	World m_World;
	Camera m_Camera;

	UI m_UI;

	UIGroup m_Popup;

	UIRectComponent* m_FadeIn;

	SDL_Event m_Event{};

	const uint8_t* m_KeyboardState = nullptr;

	int m_Width, m_Height;

	AudioSource m_FootstepSFX;
	const char* m_FootstepSFXOptions[4] = { "res/sounds/footstep_2.wav", "res/sounds/footstep_3.wav", "res/sounds/footsteprun_2.wav", "res/sounds/footsteprun_3.wav" };

};
