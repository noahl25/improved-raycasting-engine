#pragma once

#include "State.h"
#include "UI.h"
#include "Renderer.h"
#include "Audio.h"

class MainMenu : public State {
	
public:

	MainMenu(const Renderer& renderer, int width, int height);
	void HandleEvents(float deltaTime) override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) override;
	
private:

	const Renderer& m_Renderer;

	UI m_UI;

	UIGroup m_Popup;
	UIGroup m_MainMenuTitle;
	UIGroup m_MainMenuButtons;

	UIImageComponent* m_MainMenuBGImage;

	UIRectComponent* m_Overlay;

	SDL_Event m_Event;

	int m_Width, m_Height;

	bool m_Complete = false;

};