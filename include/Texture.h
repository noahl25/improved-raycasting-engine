#pragma once

#include <glm/glm.hpp>
#include <string>

#include "stb_image.h"
#include "Util.h"

class Texture
{
public:

	Texture(const char* path);
	Texture(const Texture& other);
	Texture() = default;
	~Texture();

	Texture& operator=(const Texture& other);

	glm::vec4 PixelAtVec(int x, int y) const;

	inline uint32_t PixelAt(int x, int y) const {
		return m_LookupTable[y * m_Width + x];
	}

	void SetTexture(const char* path);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

private:

	uint32_t* m_LookupTable = nullptr;
	int m_Width, m_Height;
};

