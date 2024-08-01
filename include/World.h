#pragma once

#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "Sprite.h"
#include "Ray.h"
#include "Color.h"
#include "Texture.h"
#include "Renderer.h"

class World {

public:
	
	~World();

	void LoadRoom(const char* path, const char* config);
	void LoadSprites(const char* path, const char* config);
	RaycastHit CastRay(const Ray& ray);

	inline bool Collision(int x, int y) const {
		if (m_Room[y * m_MapWidth + x] == 1)
			return true;
		return false;
	}
	inline const std::vector<Sprite>& GetSpritesSorted(const Camera& camera) { 
		std::sort(m_Sprites.begin(), m_Sprites.end(), [camera](const Sprite& first, const Sprite& second) {
			float firstDist = (first.GetPosition().x - camera.Position.x) * (first.GetPosition().x - camera.Position.x) + (first.GetPosition().y - camera.Position.y) * (first.GetPosition().y - camera.Position.y);
			float secondDist = (second.GetPosition().x - camera.Position.x) * (second.GetPosition().x - camera.Position.x) + (second.GetPosition().y - camera.Position.y) * (second.GetPosition().y - camera.Position.y);
			return firstDist > secondDist;
		});
		return m_Sprites;
	}
	inline const glm::vec2& GetStartingPos() const { return m_PlayerStartingPos; }
	inline const Texture& GetTextureAtPos(int x, int y) { 
		return m_RoomTextureAtlas[m_Room[y * m_MapWidth + x]];
	}

private:

	std::vector<Sprite> m_Sprites;
	std::vector<int> m_Room;
	//0-14 for walls 15-29 for floors
	Texture m_RoomTextureAtlas[30];

	int m_MapWidth, m_MapHeight;

	glm::vec2 m_PlayerStartingPos;

};