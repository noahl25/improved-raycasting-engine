#include "Texture.h"
#include <iostream>

Texture::Texture(const char* path)
{
	int channels;

	unsigned char* data = stbi_load(path, &m_Width, &m_Height, &channels, 4);
	if (data == NULL) __debugbreak();

	

	m_Data = data;

 
}

Texture::Texture(const Texture& other)
{
	m_Width = other.m_Width;
	m_Height = other.m_Height;

	m_Data = new unsigned char[m_Width * m_Height * sizeof(uint32_t)];
	std::memcpy(m_Data, other.m_Data, m_Width * m_Height * sizeof(uint32_t));
}

Texture::Texture()
{
}

glm::vec4 Texture::PixelAt(int x, int y) const
{
	if (m_Data == nullptr) __debugbreak();
	glm::vec4 pixel = glm::vec4(0.0f);
	pixel.r = (int)m_Data[(y * m_Width + x) * 4] / 255.0f;
	pixel.g = (int)m_Data[(y * m_Width + x) * 4 + 1] / 255.0f;
	pixel.b = (int)m_Data[(y * m_Width + x) * 4 + 2] / 255.0f;
	pixel.a = (int)m_Data[(y * m_Width + x) * 4 + 3] / 255.0f;
	return pixel;
}

void Texture::SetTexture(const char* path)
{
	int channels;

	unsigned char* data = stbi_load(path, &m_Width, &m_Height, &channels, 4);
	if (data == NULL) __debugbreak();

	m_Data = data;
}

Texture::~Texture()
{
	stbi_image_free(m_Data);
}