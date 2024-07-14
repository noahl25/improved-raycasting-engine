#include "Texture.h"
 
Texture::Texture(const char* path)
{	
 
	SetTexture(path);
 
}

Texture::Texture(const Texture& other)
{
	m_Width = other.m_Width;
	m_Height = other.m_Height;

	m_LookupTable = new uint32_t[m_Width * m_Height];
	for (int i = 0; i < m_Width * m_Height; i++)
		m_LookupTable[i] = other.m_LookupTable[i];

}

void Texture::SetTexture(const char* path)
{
	int channels;

	unsigned char* data = stbi_load(path, &m_Width, &m_Height, &channels, 4);
	if (data == NULL) __debugbreak();

	if (m_LookupTable != nullptr)
		delete[] m_LookupTable;

	m_LookupTable = new uint32_t[m_Width * m_Height];
	for (int i = 0; i < m_Width * m_Height; i++) {
		int index = i * 4;
		m_LookupTable[i] = (data[index] << 24) | (data[index + 1] << 16) | (data[index + 2] << 8) | data[index + 3];
	}

	stbi_image_free(data);

}

Texture& Texture::operator=(const Texture& other)
{
	m_Width = other.m_Width;
	m_Height = other.m_Height;

	m_LookupTable = new uint32_t[m_Width * m_Height];
	for (int i = 0; i < m_Width * m_Height; i++)
		m_LookupTable[i] = other.m_LookupTable[i];

	return *this;
}

glm::vec4 Texture::PixelAtVec(int x, int y) const
{
	if (m_LookupTable == nullptr) __debugbreak();
	glm::vec4 pixel = glm::vec4(0.0f);
	uint32_t color = m_LookupTable[y * m_Width + x];
	pixel.r = ((color >> 24) & 0xFF) / 255.0f;
	pixel.g = ((color >> 16) & 0xFF) / 255.0f;
	pixel.b = ((color >> 8) & 0xFF) / 255.0f;
	pixel.a = (color & 0xFF) / 255.0f;
	return pixel;
}

Texture::~Texture()
{
	if (m_LookupTable != nullptr)
		delete[] m_LookupTable;
}

