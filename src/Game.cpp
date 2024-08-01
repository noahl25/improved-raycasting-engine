#include "Game.h"

Game::Game(SDL_Window* window, int width, int height)
	: m_Width(width), m_Height(height), m_Renderer(width, height), m_Raycaster(m_Renderer), m_UI(m_Renderer)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);

	m_Renderer.Init(window);
	m_Renderer.CreateRenderingSurface();

	m_Raycaster.SetActiveWorld(&m_World);

	m_World.LoadRoom("res/maps/map1_room.png", "res/maps/config_room.txt");
	m_World.LoadSprites("res/maps/map1_sprites.png", "res/maps/config_sprites.txt");

	m_Camera.Position = m_World.GetStartingPos();

	m_UI.RegisterUIComponent<UIRectComponent>(SDL_Rect{ m_Renderer.GetWidth() / 2 - 5, m_Renderer.GetHeight() / 2 - 1, 10, 2 }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	m_UI.RegisterUIComponent<UIRectComponent>(SDL_Rect{ m_Renderer.GetWidth() / 2 - 1, m_Renderer.GetHeight() / 2 - 5, 2, 10 }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	Text text;
	text.Chars = "some text hi hi hi hi hi hi hi hi hi";
	text.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	text.Location = { 40, 0 };
	text.Wrap = 400;

	m_Popup.AddComponent(m_UI.RegisterUIComponent<UITypewriterTextComponent>("res/fonts/PixelifySans-Regular.ttf", 30, text, 5));
	m_Popup.Get<UITextComponent>().Get(FIELD(Text, Location)).y = m_Renderer.GetHeight() - m_Popup.Get<UITextComponent>().SizeText("some text hi hi hi hi hi hi hi hi hi").h - 40;
	m_Popup.AddComponent(m_UI.RegisterUIComponent<UIRectComponent>(Util::PadRect(m_Popup.Get<UITextComponent>().SizeText("some text hi hi hi hi hi hi hi hi hi"), 21), glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	m_Popup.AddComponent(m_UI.RegisterUIComponent<UIRectComponent>(Util::PadRect(m_Popup.Get<UITextComponent>().SizeText("some text hi hi hi hi hi hi hi hi hi"), 20), glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f }));

	m_UI.MoveToTop(&m_Popup.Get<UITextComponent>());
	
	m_Popup.Hide();

	m_FadeIn = m_UI.RegisterUIComponent<UIRectComponent>(SDL_Rect{ 0, 0, width, height }, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

}

 
void Game::HandleEvents(float deltaTime)
{
	const float lookSpeedX = 0.5f;
	const float lookSpeedY = 8.0f;
 
	while ((SDL_PollEvent(&m_Event)) != 0)
	{
		switch (m_Event.type) {

		case SDL_QUIT:
			std::exit(0);
			break;
		case SDL_MOUSEMOTION:
			m_Camera.Yaw = Util::Lerp(m_Camera.Yaw, m_Camera.Yaw + m_Event.motion.xrel * lookSpeedX, deltaTime * 4.0f);
			m_Camera.Pitch = Util::Lerp(m_Camera.Pitch, m_Camera.Pitch - m_Event.motion.yrel * lookSpeedY, deltaTime * 4.0f);
			m_Camera.Pitch = std::min(std::max(m_Camera.Pitch, -m_Height / 2.0f), m_Height / 2.0f);
			break;
		case SDL_KEYDOWN:
			if (m_Event.key.keysym.sym == SDLK_ESCAPE) 
				std::exit(0);
			break;
		}
	}

	m_KeyboardState = SDL_GetKeyboardState(NULL);

}


void Game::Update(float deltaTime)
{

	const float moveSpeed = 0.35f;

	const glm::vec2 forward = glm::normalize(glm::vec2(glm::cos(glm::radians(m_Camera.Yaw)), glm::sin(glm::radians(m_Camera.Yaw))));
	const glm::vec2 right = glm::normalize(glm::vec2(-glm::sin(glm::radians(m_Camera.Yaw)), glm::cos(glm::radians(m_Camera.Yaw))));

	glm::vec2 nextMove = glm::vec2(0.0f, 0.0f);

	if (m_KeyboardState[SDL_SCANCODE_W]) nextMove += forward;
	if (m_KeyboardState[SDL_SCANCODE_S]) nextMove -= forward;
	if (m_KeyboardState[SDL_SCANCODE_A]) nextMove -= right;
	if (m_KeyboardState[SDL_SCANCODE_D]) nextMove += right;

	if (nextMove.x != 0 || nextMove.y != 0) {

		nextMove = glm::normalize(nextMove);
		nextMove *= m_KeyboardState[SDL_SCANCODE_LSHIFT] ? moveSpeed * 2.5f : moveSpeed;
		nextMove *= deltaTime;

		if (!m_World.Collision((int)(m_Camera.Position.x + nextMove.x * 4), (int)m_Camera.Position.y))
			m_Camera.Position.x = m_Camera.Position.x + nextMove.x;
		if (!m_World.Collision((int)m_Camera.Position.x, (int)(m_Camera.Position.y + nextMove.y * 4)))
			m_Camera.Position.y = m_Camera.Position.y + nextMove.y;

	}

	if (Sprite::HoveredSprite && Sprite::HoveredSpriteDistance <= 1.0f) {
		m_Popup.Show();
		m_Popup.Get<UITypewriterTextComponent>().Write();
	}
	else if (!Sprite::HoveredSprite) {
		m_Popup.Hide();
		m_Popup.Get<UITypewriterTextComponent>().Reset();
	}
	static float fadeAlpha = 1.0f;

	if (m_FadeIn && m_FadeIn->GetColor().a > 0.0f) {
		m_FadeIn->SetAlpha(fadeAlpha);
		fadeAlpha -= 0.001f;
		if (fadeAlpha < 0.0f) {
			m_FadeIn->Destroy();
			m_FadeIn = nullptr;
		}
			
	}
	m_UI.Update();
}


void Game::Render(float deltaTime)
{
	m_Renderer.Clear();

	m_Raycaster.Draw(m_Camera);
	m_UI.Draw();

	m_Renderer.Present();

}


Game::~Game()
{
 
}