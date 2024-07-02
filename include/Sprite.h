#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "Texture.h"

struct Sprite {
public:

	Sprite();
	Sprite(const glm::vec3& position);
	~Sprite();

	void SetTexture(SDL_Renderer* renderer, const char* path);

	virtual void Update();
	virtual void Draw(Renderer& renderer, const Camera& camera, uint32_t* pixels, float* zBuffer) const;
	
	inline void Kill() { m_Active = false; }

private:

	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	bool m_Active = true;
	Texture test;

	int m_SpriteWidth, m_SpriteHeight;

	const float FALLOFF = 0.23f;

};