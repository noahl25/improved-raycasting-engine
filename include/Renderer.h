#pragma once

#include <SDL.h>
#include <cassert>
#include <glm/glm.hpp>

#include "Util.h"

class Renderer
{
public:

	Renderer(int width, int height);
	~Renderer();

	void Init(SDL_Window* window);

	void Present() const;
	void Clear(const glm::vec4& color = glm::vec4(0.0f)) const;

	void Rect(const SDL_Rect& rect, const glm::vec4& color = glm::vec4(1.0f)) const;
	void Line(const glm::vec2& first, const glm::vec2& second, const glm::vec4& color = glm::vec4(1.0f)) const;
	void Texture(SDL_Texture* texture, const SDL_Rect* dest, const SDL_Rect* src = NULL) const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline SDL_Renderer* GetRenderer() const { return m_Renderer; }
	inline void Blit(SDL_Texture* texture) const { SDL_RenderCopy(m_Renderer, texture, NULL, NULL); }
	
	static inline SDL_Renderer* GetActiveRenderer() { return m_ActiveRenderer; }

private:

	SDL_Renderer* m_Renderer;

	int m_Width, m_Height;

	static inline SDL_Renderer* m_ActiveRenderer = nullptr;

};

