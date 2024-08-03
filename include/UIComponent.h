#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <format>
#include <functional>

#include "FontManager.h"
#include "Renderer.h"
#include "Animation.h"

class UIComponent {

public:

	virtual void Update() {}
	virtual void Draw(const Renderer& renderer) const = 0;
	virtual ~UIComponent() = default;

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

	inline void Set(const SDL_Rect& rect) { m_Rect = rect; }

	inline const glm::vec4& GetColor() { return m_Color; }

private:

	glm::vec4 m_Color;
	SDL_Rect m_Rect;

};

class UITextComponent;

struct Text {

	std::string Chars;
	glm::vec2 Location;
	glm::vec4 Color;

	int Wrap = -1;
};

class UITextComponent : public UIComponent {

public:

	UITextComponent(const char* font, int fontSize, const Text& text);
	virtual void Update() override;
	virtual void Draw(const Renderer& renderer) const override;
	
	//use sparingly
	SDL_Rect SizeText(const char* text);

	inline void Center(int w, int h) { 
		SDL_Rect rect = SizeText(m_Text.Chars.c_str());
		m_Text.Location.x = w / 2 - rect.w / 2;
		m_Text.Location.y = h / 2 - rect.h / 2;
		m_Updated = true;
	}

	inline void SetText(std::string&& text) { 
		m_Text.Chars = std::move(text); 
		m_Updated = true;
	}

	inline void SetText(const std::string& text) {
		m_Text.Chars = text;
		m_Updated = true;
	}

	inline void SetStyle(int style) {
		TTF_SetFontStyle(m_Font, style);
		m_Updated = true;
	}

	inline void SetAlign(int align) {
		TTF_SetFontWrappedAlign(m_Font, align);
		m_Updated = true;
	}

	inline void SetOutline(int outline) {
		TTF_SetFontOutline(m_Font, outline);
	}

	inline void AddBackground(const glm::vec4& color, int margin) { m_TextBackgrounds.push_back({ false, SDL_Rect{}, color, margin }); }

	inline const SDL_Rect& GetTextBounds() const { return m_TextRect; }

	template <typename T>
	inline T& Get(T Text::* member) {
		m_Updated = true;
		return m_Text.*member;
	}

	virtual ~UITextComponent();

private:

	struct TextBackground {
		mutable bool RectCached;
		mutable SDL_Rect Rect;
		glm::vec4 m_Color;
		int Margin;
	};

	void RenderBackgrounds(const Renderer& renderer) const;

private:

	SDL_Texture* m_TextTexture = nullptr;

	int m_LineSkip;

	std::vector<TextBackground> m_TextBackgrounds;

	static inline FontManager Fonts;

protected:

	SDL_Rect m_TextRect{};
	Text m_Text;
	TTF_Font* m_Font;
	mutable bool m_Updated = true;
	
};

class UITypewriterTextComponent : public UITextComponent {

public:

	UITypewriterTextComponent(const char* font, int fontSize, const Text& text, int speed);
	virtual void Update() override;

	inline void Reset() { 
		m_CurrentText = ""; 
		m_CurrentIndex = 0; 
		m_Writing = false;
		SetText("");
	}
	
	inline void Write() { m_Writing = true; }

	inline void SetEndText(const std::string& str) { Reset(); m_EndText = str; m_EndTextBounds = SizeText(str.c_str()); }

	inline void RecalculateEndTextBounds() { m_EndTextBounds = SizeText(m_EndText.c_str()); }
	inline const SDL_Rect& GetEndTextBounds() { return m_EndTextBounds; }

private:

	std::string m_EndText;
	std::string m_CurrentText = "";
	int m_CurrentIndex = 0;
	int m_Speed;

	bool m_Writing = false;

	SDL_Rect m_EndTextBounds;

};

class UIGroup {

private:

	template <typename T>
	static constexpr bool InheritsFromUIComponent = std::is_base_of_v<UIComponent, T>&& Util::is_different_v<UIComponent, T>;

public:

	void AddComponent(UIComponent* component);

	void Destroy();
	void Hide();
	void Show();

	template <typename T> requires InheritsFromUIComponent<T>
	T& Get(int index) const {
		T* comp = dynamic_cast<T*>(m_UIComponents[index]);
		if (comp == nullptr)
			throw std::runtime_error(std::format("component at index {} is not of type {}", index, typeid(T).name()));
		return *comp;
	}

	template <typename T> requires InheritsFromUIComponent<T>
	T& Get() const {
		for (auto& component : m_UIComponents) {
			T* comp = dynamic_cast<T*>(component);
			if (comp != nullptr)
				return *comp;
		}
		throw std::runtime_error(std::string("unable to find type in UIGroup: ") + typeid(T).name());

	}

	template <typename T> requires InheritsFromUIComponent<T>
	void ForEach(const std::function<void(T& it)>& func) {
		for (auto& component : m_UIComponents) {
			T* comp = dynamic_cast<T*>(component);
			if (comp != nullptr) {
				func(*comp);
			}
		}
	}

private:

	std::vector<UIComponent*> m_UIComponents;

};

class UIImageComponent : public UIComponent {

public:

	UIImageComponent(const char* path, const SDL_Rect& dest);
	virtual void Draw(const Renderer& renderer) const override;

	inline void SetSource(const SDL_Rect& src) {
		m_Src = src;
		m_HasSrc = true;
	}

	inline void SetColorMod(const SDL_Color& color) { 
		m_ColorMod = color; 
		m_HasColorMod = true;
	}

	virtual ~UIImageComponent();

private:

	SDL_Texture* m_Texture;
	SDL_Rect m_Dest;

	SDL_Color m_ColorMod;
	bool m_HasColorMod = false;

	bool m_HasSrc = false;
	SDL_Rect m_Src;

};

class UITextButtonComponent : public UITextComponent {

public:

	UITextButtonComponent(const char* font, int fontSize, const Text& text, const std::function<void(UITextButtonComponent& it)>& callback);
	virtual void Update() override;

	inline void SetOnHover(const std::function<void(UITextButtonComponent& it)>& callback) {
		m_OnHover = callback;
	}
	inline void SetOnLeaveHover(const std::function<void(UITextButtonComponent& it)>& callback) {
		m_OnLeaveHover = callback;
	}

	inline void RecalculateBoundingBox() {
		m_BoundingBox = SizeText(m_Text.Chars.c_str());
	}

	inline void SetBoundingBoxStatic() {
		m_StaticBoundingBox = true;
	}

private:

	std::function<void(UITextButtonComponent& it)> m_Callback = nullptr;
	std::function<void(UITextButtonComponent& it)> m_OnHover = nullptr;
	std::function<void(UITextButtonComponent& it)> m_OnLeaveHover = nullptr;

	SDL_Rect m_BoundingBox;

	bool m_Clicked = false;
	bool m_Hovering = false;

	bool m_StaticBoundingBox = false;
};