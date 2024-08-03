#include "MainMenu.h"

static void MoveTitle(UITextComponent& comp) {
	static float inc = 0.0f;
	comp.Get(&Text::Location).y -= inc;
	inc += 0.01f;
}

static void FadeInButton(UITextButtonComponent& comp) {
	float& alpha = comp.Get(&Text::Color).a;
	alpha += 0.01f;
	if (alpha > 1.0f)
		alpha = 1.0f;
}

MainMenu::MainMenu(const Renderer& renderer, int width, int height)
	: m_Renderer(renderer), m_UI(m_Renderer), m_Width(width), m_Height(height)
{
	m_MainMenuBGImage = m_UI.RegisterUIComponent<UIImageComponent>("res/textures/MainMenu.png", SDL_Rect{ 0, 0, 750, 650 });
	m_MainMenuBGImage->SetColorMod({ 0, 0, 0, 255 });

	Text text1;
	text1.Chars = "ESCAPE";
	text1.Color = { 0.5f, 0.5f, 0.5f, 1.0f };
	text1.Location = { 100, 100 };
	text1.Wrap = -1;
	m_MainMenuTitle.AddComponent(m_UI.RegisterUIComponent<UITextComponent>("res/fonts/PixelifySans-Regular.ttf", 60, text1));
	text1.Color = { 0.3f, 0.3f, 0.3f, 1.0f };
	m_MainMenuTitle.AddComponent(m_UI.RegisterUIComponent<UITextComponent>("res/fonts/PixelifySans-Regular.ttf", 60, text1));

	m_UI.MoveToTop(&m_MainMenuTitle.Get<UITextComponent>(0));

	m_MainMenuTitle.Get<UITextComponent>(1).SetOutline(3);
	m_MainMenuTitle.ForEach<UITextComponent>([this](UITextComponent& comp) {
		comp.SetStyle(TTF_STYLE_BOLD);
		comp.Center(m_Width, m_Height);
	});


	m_MainMenuTitle.Get<UITextComponent>(1).Get(FIELD(Text, Location)).x -= 2;
	m_MainMenuTitle.Get<UITextComponent>(1).Get(FIELD(Text, Location)).y += 2;

	Text buttonText;
	buttonText.Chars = "Start";
	buttonText.Color = { 1.0f, 1.0f, 1.0f, 0.0f };
	buttonText.Location = { 100, 100 };

	m_MainMenuButtons.AddComponent(m_UI.RegisterUIComponent<UITextButtonComponent>("res/fonts/PixelifySans-Regular.ttf", 20, buttonText, [this](UITextButtonComponent& it) { Audio::SetSoundEffectVolume("res/sounds/buttonpressed.wav", 10); Audio::PlaySoundEffect("res/sounds/buttonpressed.wav"); Complete(); }));
	buttonText.Chars = "Quit";
	m_MainMenuButtons.AddComponent(m_UI.RegisterUIComponent<UITextButtonComponent>("res/fonts/PixelifySans-Regular.ttf", 20, buttonText, [](UITextButtonComponent& it) { std::exit(0); }));

	m_MainMenuButtons.ForEach<UITextButtonComponent>([this](UITextButtonComponent& comp) {
		comp.Center(m_Width, m_Height);
		comp.SetBoundingBoxStatic();
	});

	m_MainMenuButtons.Get<UITextButtonComponent>(0).Get(&Text::Location).y -= 15.0f;
	m_MainMenuButtons.Get<UITextButtonComponent>(1).Get(&Text::Location).y += 15.0f;

	m_MainMenuButtons.ForEach<UITextButtonComponent>([this](UITextButtonComponent& comp) {
		comp.RecalculateBoundingBox();
	});

	m_MainMenuButtons.Get<UITextButtonComponent>(0).SetOnHover([](UITextButtonComponent& comp) {
		static int originalPos = comp.Get(&Text::Location).y;
		comp.Get(&Text::Location).y = originalPos + 5 * glm::sin(0.1f * Animation::Counter());
		comp.Get(&Text::Color).b = 0.5f;
		comp.Get(&Text::Color).r = 0.5f;
		comp.Get(&Text::Color).g = 0.5f;
	});
	m_MainMenuButtons.Get<UITextButtonComponent>(0).SetOnLeaveHover([this](UITextButtonComponent& comp) {
		comp.Center(m_Width, m_Height);
		comp.Get(&Text::Location).y -= 15.0f;
		comp.Get(&Text::Color).b = 1.0f;
		comp.Get(&Text::Color).r = 1.0f;
		comp.Get(&Text::Color).g = 1.0f;
	});

	m_MainMenuButtons.Get<UITextButtonComponent>(1).SetOnHover([](UITextButtonComponent& comp) {
		static int originalPos = comp.Get(&Text::Location).y;
		comp.Get(&Text::Location).y = originalPos + 5 * glm::sin(0.1f * Animation::Counter());
		comp.Get(&Text::Color).b = 0.5f;
		comp.Get(&Text::Color).r = 0.5f;
		comp.Get(&Text::Color).g = 0.5f;
	});
	m_MainMenuButtons.Get<UITextButtonComponent>(1).SetOnLeaveHover([this](UITextButtonComponent& comp) {
		comp.Center(m_Width, m_Height);
		comp.Get(&Text::Location).y += 15.0f;
		comp.Get(&Text::Color).b = 1.0f;
		comp.Get(&Text::Color).r = 1.0f;
		comp.Get(&Text::Color).g = 1.0f;
	});

	m_MainMenuButtons.Hide();
}
 
void MainMenu::HandleEvents(float deltaTime)
{

	while ((SDL_PollEvent(&m_Event)) != 0) {
		switch (m_Event.type) {
			case SDL_QUIT:
				std::exit(0);
				break;

		}
	}
}

void MainMenu::Update(float deltaTime)
{
	static int bgColorCounter = 0;

	if (m_MainMenuTitle.Get<UITextComponent>(0).Get(&Text::Location).y > 200 && Animation::Counter() > 100) {
		m_MainMenuTitle.ForEach<UITextComponent>(&MoveTitle);
	}
	else if (Animation::Counter() > 300) {
		m_MainMenuButtons.Show();
		DO_ONCE(m_MainMenuButtons.ForEach<UITextButtonComponent>([](UITextButtonComponent& it) { it.Get(&Text::Color).a = 0.0f; }));
		m_MainMenuButtons.ForEach<UITextButtonComponent>(&FadeInButton);
		uint8_t mod = glm::abs(65 * glm::sin(0.01f * bgColorCounter));
		bgColorCounter++;
		m_MainMenuBGImage->SetColorMod({ mod, mod, mod, 255 });
	}

	m_UI.Update();
}

void MainMenu::Render(float deltaTime)
{
	m_Renderer.Clear();

	m_UI.Draw();

	m_Renderer.Present();
}
