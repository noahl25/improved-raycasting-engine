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
	: m_Text(text)
{
	m_Text.Font = TTF_OpenFont(font, fontSize);

}

void UITextComponent::Draw(const Renderer& renderer) const
{
	if (m_Updated) {

		SDL_DestroyTexture(m_TextTexture);

		SDL_Color color = { (int)(m_Text.Color.r * 255.0f), (int)(m_Text.Color.g * 255.0f), (int)(m_Text.Color.b * 255.0f), (int)(m_Text.Color.a * 255.0f) };
		SDL_Surface* textSurface = nullptr;

		m_TextRect.x = m_Text.Location.x;
		m_TextRect.y = m_Text.Location.y;

		if (m_Text.Wrap < 0) {
			textSurface = TTF_RenderText_Solid(m_Text.Font, m_Text.Chars.c_str(), color);

			TTF_SizeText(m_Text.Font, m_Text.Chars.c_str(), &m_TextRect.w, &m_TextRect.h);
		}
		else {
			textSurface = TTF_RenderText_Solid_Wrapped(m_Text.Font, m_Text.Chars.c_str(), color, m_Text.Wrap);

			int numCharsOnLine;
			TTF_MeasureText(m_Text.Font, m_Text.Chars.c_str(), m_Text.Wrap, NULL, &numCharsOnLine);

			m_TextRect.w = m_Text.Wrap;
			m_TextRect.h = (TTF_FontHeight(m_Text.Font) + TTF_FontLineSkip(m_Text.Font)) * (m_Text.Chars.length() / numCharsOnLine);
		}

		m_TextTexture = SDL_CreateTextureFromSurface(Renderer::GetActiveRenderer(), textSurface);
		renderer.Texture(m_TextTexture, &m_TextRect);

		SDL_FreeSurface(textSurface);

		m_Updated = false;

	}

	else {
		renderer.Texture(m_TextTexture, &m_TextRect);
	}
}

UITextComponent::~UITextComponent()
{
	TTF_CloseFont(m_Text.Font);
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
