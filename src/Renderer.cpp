#include "Renderer.h"

Renderer::Renderer(int width, int height)
	: m_Width(width), m_Height(height)
{

}

void Renderer::Init(SDL_Window* window) {
	assert(window != nullptr);
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	assert(m_Renderer != nullptr);
	m_ActiveRenderer = m_Renderer;
	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
}

void Renderer::Present() const {

	SDL_RenderPresent(m_Renderer);
}

void Renderer::Clear(const glm::vec4& color) const {
	uint32_t asUint = Util::AsUInt32(color);
	SDL_SetRenderDrawColor(m_Renderer, EXPAND_UINT32(asUint));
	SDL_RenderClear(m_Renderer);
}

void Renderer::Rect(const SDL_Rect& rect, const glm::vec4& color) const {
	uint32_t asUint = Util::AsUInt32(color);
	SDL_SetRenderDrawColor(m_Renderer, EXPAND_UINT32(asUint));
	SDL_RenderFillRect(m_Renderer, &rect);
}

void Renderer::Line(const glm::vec2& first, const glm::vec2& second, const glm::vec4& color) const {
	uint32_t asUint = Util::AsUInt32(color);
	SDL_SetRenderDrawColor(m_Renderer, EXPAND_UINT32(asUint));
	SDL_RenderDrawLine(m_Renderer, first.x, first.y, second.x, second.y);
}

void Renderer::Texture(SDL_Texture* texture, const SDL_Rect* dest, const SDL_Rect* src) const
{
 
	SDL_RenderCopy(m_Renderer, texture, src, dest);
}

uint32_t* Renderer::CreateRenderingSurface()
{
	m_RenderingSurface = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	SDL_SetSurfaceBlendMode(m_RenderingSurface, SDL_BLENDMODE_BLEND);
	m_SurfacePixels = reinterpret_cast<uint32_t*>(m_RenderingSurface->pixels);
	return m_SurfacePixels;
}

void Renderer::BlitRenderingSurface() const
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, m_RenderingSurface);
	Blit(texture);
	SDL_DestroyTexture(texture);
}

void Renderer::BlitToRenderingSurface(SDL_Surface* surface, SDL_Rect& rect) const
{
	if (m_RenderingSurface == nullptr) __debugbreak();
	SDL_UpperBlit(surface, NULL, m_RenderingSurface, &rect);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_Renderer);
}
