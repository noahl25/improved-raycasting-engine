#include "UIComponent.h"


UIRectComponent::UIRectComponent(const SDL_Rect& rect, const glm::vec4& color)
	: m_Color(color), m_Rect(rect)
{
}

void UIRectComponent::Draw(const Renderer& renderer) const
{
	renderer.Rect(m_Rect, m_Color);
}

UITextComponent::UITextComponent(const char* font, int fontSize, const Text& text)
{
	m_Text = text;
	m_Font = Fonts.Get(font, fontSize);
	m_LineSkip = TTF_FontLineSkip(m_Font);
}

void UITextComponent::Update()
{
	if (m_Text.Chars.empty())
		return;

	if (m_Updated) {

		SDL_DestroyTexture(m_TextTexture);

		SDL_Color color = { (int)(m_Text.Color.r * 255.0f), (int)(m_Text.Color.g * 255.0f), (int)(m_Text.Color.b * 255.0f), (int)(m_Text.Color.a * 255.0f) };
		SDL_Surface* textSurface = nullptr;

		m_TextRect.x = m_Text.Location.x;
		m_TextRect.y = m_Text.Location.y;
		

		if (m_Text.Wrap < 0) {
			textSurface = TTF_RenderText_Solid(m_Font, m_Text.Chars.c_str(), color);
		}
		else {
			textSurface = TTF_RenderText_Solid_Wrapped(m_Font, m_Text.Chars.c_str(), color, m_Text.Wrap);
		}

		m_TextRect.w = textSurface->w;
		m_TextRect.h = textSurface->h;

		m_TextTexture = SDL_CreateTextureFromSurface(Renderer::GetActiveRenderer(), textSurface);

		SDL_FreeSurface(textSurface);

	}
}

void UITextComponent::Draw(const Renderer& renderer) const
{
	if (m_Text.Chars.empty())
		return;

	if (!m_TextBackgrounds.empty())
		RenderBackgrounds(renderer);
	renderer.Texture(m_TextTexture, &m_TextRect);
	m_Updated = false;
	
}

SDL_Rect UITextComponent::SizeText(const char* text)
{
	SDL_Surface* temp = nullptr;
	if (m_Text.Wrap > 0)
		temp = TTF_RenderText_Solid_Wrapped(m_Font, text, SDL_Color{ 0, 0, 0, 0 }, m_Text.Wrap);
	else
		temp = TTF_RenderText_Solid(m_Font, text, SDL_Color{ 0, 0, 0, 0 });
	SDL_Rect rect;
	rect.x = m_Text.Location.x;
	rect.y = m_Text.Location.y;
	rect.w = temp->w;
	rect.h = temp->h;
	SDL_FreeSurface(temp);
	return rect;
}

void UITextComponent::RenderBackgrounds(const Renderer& renderer) const
{
	for (const auto& background : m_TextBackgrounds) {
		if (!background.RectCached || m_Updated) {
			background.Rect = m_TextRect;
			background.Rect.x -= background.Margin;
			background.Rect.y -= background.Margin - 2;
			background.Rect.w += background.Margin * 1.8;
			background.Rect.h += background.Margin * 1.75;
			background.RectCached = true;
		}
		renderer.Rect(background.Rect, background.m_Color);
	}
}

UITextComponent::~UITextComponent()
{
	SDL_DestroyTexture(m_TextTexture);
}


void UIGroup::AddComponent(UIComponent* component)
{
	m_UIComponents.push_back(component);
}

void UIGroup::Destroy()
{
	for (auto& component : m_UIComponents) {
		component->Destroy();
	}
}

void UIGroup::Hide()
{
	for (auto& component : m_UIComponents) {
		component->Hide();
	}
}

void UIGroup::Show()
{
	for (auto& component : m_UIComponents) {
		component->Show();
	}
}

UITypewriterTextComponent::UITypewriterTextComponent(const char* font, int fontSize, const Text& text, int speed)
	: UITextComponent(font, fontSize, text), m_EndText(text.Chars), m_Speed(speed), m_EndTextBounds(SizeText(text.Chars.c_str()))
{
}

void UITypewriterTextComponent::Update()
{
	if (Animation::Counter() % m_Speed == 0 && m_CurrentIndex < m_EndText.size() && m_Writing) {
		m_CurrentText += m_EndText[m_CurrentIndex];
		m_CurrentIndex++;
		SetText(m_CurrentText);
	}
	UITextComponent::Update();

}


UIImageComponent::UIImageComponent(const char* path, const SDL_Rect& dest)
	: m_Texture(IMG_LoadTexture(Renderer::GetActiveRenderer(), path)), m_Dest(dest)
{
}

void UIImageComponent::Draw(const Renderer& renderer) const
{
	if (m_HasColorMod) {
		SDL_SetTextureColorMod(m_Texture, m_ColorMod.r, m_ColorMod.g, m_ColorMod.b);
		SDL_SetTextureAlphaMod(m_Texture, m_ColorMod.a);
	}
	renderer.Texture(m_Texture, &m_Dest, (m_HasSrc ? &m_Src : NULL));
}

UIImageComponent::~UIImageComponent()
{
	SDL_DestroyTexture(m_Texture);
}

UITextButtonComponent::UITextButtonComponent(const char* font, int fontSize, const Text& text, const std::function<void(UITextButtonComponent& it)>& callback)
	: UITextComponent(font, fontSize, text), m_Callback(callback), m_BoundingBox(SizeText(text.Chars.c_str()))
{
}

void UITextButtonComponent::Update()
{
	SDL_Point mouse;
	int button = SDL_GetMouseState(&mouse.x, &mouse.y);
	if (SDL_PointInRect(&mouse, &m_BoundingBox)) {
		if (m_OnHover) {
			m_OnHover(*this);
			m_Hovering = true;
		}

		if (button & SDL_BUTTON(1) && !m_Clicked) {
			m_Callback(*this);
			m_Clicked = true;
		}
	}
	else {
		if (m_OnLeaveHover && m_Hovering) {
			m_OnLeaveHover(*this);
			m_Hovering = false;
		}
	}

	if (!(button & SDL_BUTTON(1))) {
		m_Clicked = false;
	}


	UITextComponent::Update();

	if (!m_StaticBoundingBox)
		m_BoundingBox = m_TextRect;
}


