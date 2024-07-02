#pragma once

#include <glm/glm.hpp>

#include "stb_image.h"
#include "Util.h"

class Texture
{
public:

	Texture(const char* path);
	Texture(const Texture& other);
	Texture();
	~Texture();

	glm::vec4 PixelAt(int x, int y) const;
	//main difference being handled with a uint32_t so less conversions (uint32_t (data) -> vec4 (return) -> uint32_t (display))
	__forceinline uint32_t PixelAtOptimized(int x, int y) const {
		uint32_t rgbaValue;
		Util::memcpy_reverse(&rgbaValue, &m_Data[(y * m_Width + x) * 4], sizeof(uint32_t));
		return rgbaValue;
	}

	void SetTexture(const char* path);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

private:

	unsigned char* m_Data = nullptr;
	int m_Width, m_Height;
};

