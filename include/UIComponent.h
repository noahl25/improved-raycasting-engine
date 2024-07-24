#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <SDL_ttf.h>
#include <string>

#include "Renderer.h"
#include "Util.h"

class UIComponent {

public:

	virtual void Update() {}
	virtual void Draw(const Renderer& renderer) const = 0;
	virtual ~UIComponent() {}

	inline void Destroy() { m_Active = false; }
	inline bool IsActive() const { return m_Active; }

	inline void Hide() { m_Hidden = true; }
	inline void Show() { m_Hidden = false; }

	inline bool Hidden() { return m_Hidden; }

private:

	bool m_Active = true;
	bool m_Hidden = false;

};


class UIRectComponent : public UIComponent {

public:

	UIRectComponent(const SDL_Rect& rect, const glm::vec4& color);
	virtual void Draw(const Renderer& renderer) const override;

	inline void SetColor(const glm::vec4& color) { m_Color = color; }
	inline void SetAlpha(float alpha) { m_Color.a = alpha; }

	inline int& Get(int SDL_Rect::* member) {
		return m_Rect.*member;
	}

	inline const glm::vec4& GetColor() { return m_Color; }

private:

	glm::vec4 m_Color;
	SDL_Rect m_Rect;

};

class UITextComponent;

struct Text {
public:
	std::string Chars;
	glm::vec2 Location;
	glm::vec4 Color;
	int Wrap = -1;
private:
	TTF_Font* Font;

	friend class UITextComponent;
};

class UITextComponent : public UIComponent {

public:

	UITextComponent(const char* font, int fontSize, const Text& text);
	virtual void Draw(const Renderer& renderer) const override;
	virtual ~UITextComponent();

	inline void SetText(std::string&& text) { 
		m_Text.Chars = std::move(text); 
		m_Updated = true;
	}

	inline void SetText(const std::string& text) {
		m_Text.Chars = text;
		m_Updated = true;
	}

	template <typename T>
	inline T& Get(T Text::* member) {
		m_Updated = true;
		return m_Text.*member;
	}

private:

	Text m_Text;
	mutable SDL_Rect m_TextRect{};
	mutable SDL_Texture* m_TextTexture = nullptr;
	mutable bool m_Updated = true;

};

class UIGroup {

public:

	void AddComponent(UIComponent* component);

	void Destroy();
	void Hide();
	void Show();

	template <typename T> requires std::is_base_of_v<UIComponent, T> && Util::is_different_v<UIComponent, T>
	T& Get(int index) {
		T* comp = dynamic_cast<T*>(m_UIComponents[index]);
		if (comp == nullptr)
			throw std::bad_cast();
		return *comp;
	}

private:

	std::vector<UIComponent*> m_UIComponents;

};

class UIImageComponent : public UIComponent {

public:


};