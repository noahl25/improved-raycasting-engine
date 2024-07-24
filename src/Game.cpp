#include "Game.h"

Game::Game(int width, int height)
	: m_Width(width), m_Height(height), m_Renderer(width, height), m_Raycaster(m_Renderer), m_UI(m_Renderer)
{

	m_Window = SDL_CreateWindow("The Escape", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, NULL);
	m_Renderer.Init(m_Window);
	m_Renderer.CreateRenderingSurface();

	SDL_SetRelativeMouseMode(SDL_TRUE);

	m_Raycaster.SetActiveWorld(&m_World);

	m_World.LoadRoom("res/maps/map1_room.png", "res/maps/config_room.txt");
	m_World.LoadSprites("res/maps/map1_sprites.png", "res/maps/config_sprites.txt");

	m_Camera.Position = m_World.GetStartingPos();

	m_UI.RegisterUIComponent<UIRectComponent>(SDL_Rect{ m_Renderer.GetWidth() / 2 - 5, m_Renderer.GetHeight() / 2 - 1, 10, 2 }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	m_UI.RegisterUIComponent<UIRectComponent>(SDL_Rect{ m_Renderer.GetWidth() / 2 - 1, m_Renderer.GetHeight() / 2 - 5, 2, 10 }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	//m_Popup.AddComponent(m_UI.RegisterUIComponent<UIRectComponent>(SDL_Rect{ 10, 10, 198, 20 }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	//m_Popup.AddComponent(m_UI.RegisterUIComponent<UIRectComponent>(SDL_Rect{ 12, 12, 194, 16 }, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f }));

	Text text;
	text.Chars = "It's just a barrel.";
	text.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	text.Location = { 14, 14 };
	text.Wrap = 192;

	m_Popup.AddComponent(m_UI.RegisterUIComponent<UITextComponent>("res/fonts/PixelifySans-Regular.ttf", 30, text));
	
	m_Popup.Hide();

}

void Game::Run()
{

	const int FPSTarget = 60;
	const int deltaTimeTarget = 1000 / FPSTarget;

	unsigned frameStart;
	unsigned frameTime;

	unsigned prevTime = SDL_GetTicks();

	while (m_Running) {

		frameStart = SDL_GetTicks();
		HandleEvents();
		if (!m_Paused) {

			HandleEvents();
			Render();

			frameTime = SDL_GetTicks() - frameStart;
			m_DeltaTime = frameTime / 1000.0f;

		}

	}


}
 
void Game::HandleEvents()
{
 
	const float moveSpeed = 0.35f;
	const float lookSpeedX = 0.5f;
	const float lookSpeedY = 8.0f;
 
	while ((SDL_PollEvent(&m_Event)) != 0)
	{
		switch (m_Event.type) {

		case SDL_QUIT:
			m_Running = false;
			break;
		case SDL_KEYDOWN:
			if (m_Event.key.keysym.sym == SDLK_ESCAPE)
				m_Running = false;
			break;
		case SDL_MOUSEMOTION:
			m_Camera.Yaw = Util::Lerp(m_Camera.Yaw, m_Camera.Yaw + m_Event.motion.xrel * lookSpeedX, m_DeltaTime * 4.0f);
			m_Camera.Pitch = Util::Lerp(m_Camera.Pitch, m_Camera.Pitch - m_Event.motion.yrel * lookSpeedY, m_DeltaTime * 4.0f);
			m_Camera.Pitch = std::min(std::max(m_Camera.Pitch, -m_Height / 2.0f), m_Height / 2.0f);
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	const glm::vec2 forward = glm::normalize(glm::vec2(glm::cos(glm::radians(m_Camera.Yaw)), glm::sin(glm::radians(m_Camera.Yaw))));
	const glm::vec2 right = glm::normalize(glm::vec2(-glm::sin(glm::radians(m_Camera.Yaw)), glm::cos(glm::radians(m_Camera.Yaw))));

	glm::vec2 nextMove = glm::vec2(0.0f, 0.0f);

	if (keyState[SDL_SCANCODE_W]) nextMove += forward;
	if (keyState[SDL_SCANCODE_S]) nextMove -= forward;
	if (keyState[SDL_SCANCODE_A]) nextMove -= right;
	if (keyState[SDL_SCANCODE_D]) nextMove += right;

	if (nextMove.x != 0 || nextMove.y != 0) {

		nextMove = glm::normalize(nextMove);
		nextMove *= moveSpeed;
		nextMove *= m_DeltaTime;

		if (!m_World.Collision((int)(m_Camera.Position.x + nextMove.x * 4), (int)m_Camera.Position.y))
			m_Camera.Position.x = m_Camera.Position.x + nextMove.x;
		if (!m_World.Collision((int)m_Camera.Position.x, (int)(m_Camera.Position.y + nextMove.y * 4)))
			m_Camera.Position.y = m_Camera.Position.y + nextMove.y;

	}

	if (Sprite::HoveredSprite && Sprite::HoveredSpriteDistance <= 1.0f)
		m_Popup.Show();
	else
		m_Popup.Hide();

	m_UI.Update();


}


void Game::Render()
{
	m_Renderer.Clear();

	m_Raycaster.Draw(m_Camera);
	m_UI.Draw();

	m_Renderer.Present();
}


Game::~Game()
{
	SDL_DestroyWindow(m_Window);
}