#pragma once

#include <vector>
#include <SDL_image.h> 

#include "Sprite.h"
#include "Ray.h"
#include "Color.h"
#include "Texture.h"
#include "Renderer.h"

class World {

public:
	
	~World();

	void LoadWorld(const char* path);
	RaycastHit CastRay(const Ray& ray);

	inline bool Collision(int x, int y) {
		if (m_Map[y * m_MapWidth + x] == 1)
			return true;
		return false;
	}
	inline const std::vector<Sprite>& GetSprites() const { return m_Sprites; }
	inline const glm::vec2& GetStartingPos() const { return m_PlayerStartingPos; }

private:

	enum Tile {

	};

	std::vector<Sprite> m_Sprites;
	std::vector<int> m_Map;

	int m_MapWidth, m_MapHeight;

	glm::vec2 m_PlayerStartingPos;

};