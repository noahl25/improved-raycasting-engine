#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

#include "Renderer.h"
#include "Camera.h"
#include "Texture.h"

class World;

class Sprite {
public:

	Sprite();
	Sprite(const Sprite& other);
	Sprite(Sprite&& other) noexcept;
	Sprite& operator=(const Sprite& other);
	Sprite& operator=(Sprite&& other) noexcept;
	Sprite(const glm::vec3& position);
	~Sprite();

	void SetTexture(SDL_Renderer* renderer, const std::string& path);

	virtual void Update();
	virtual void Draw(const Renderer& renderer, const Camera& camera, uint32_t* pixels, float* zBuffer) const;
	
	inline void Kill() { m_Active = false; }
	inline const glm::vec3& GetPosition() const { return m_Position; }
	inline void SetPosition(const glm::vec3& position) { m_Position = position; }
	inline void SetScale(float scale) { m_Scale = scale; }
	inline void SetHeightOffset(float offset) { m_HeightOffset = offset; }
	inline const std::string& GetIdentifier() { return m_Identifier; }

	//rather than raycasting to see what sprites are on crosshair, simply check when drawing to see if any pixels overlap with the center
	static inline const Sprite* HoveredSprite = nullptr;
	static inline float HoveredSpriteDistance = 0.0f;

private:

	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	bool m_Active = true;
	bool m_Grounded = true;
	float m_Scale = 1.0f;
	float m_HeightOffset = 0.0f;
	int m_TextureKey;
	int m_SpriteWidth, m_SpriteHeight;

	uint32_t* m_ModificationTable = nullptr;

	std::string m_Identifier;

	static inline const float m_Falloff = 0.23f;
	static inline const std::hash<std::string> m_StringHash;
	static inline std::unordered_map<int, Texture> m_TextureAtlas;

private:

	void CopyBasicMembers(Sprite& first, const Sprite& second);

	friend class World;

};