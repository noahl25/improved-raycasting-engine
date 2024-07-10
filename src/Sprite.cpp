#include "Sprite.h"
#include <iostream>
Sprite::Sprite()
{
}

Sprite::Sprite(const glm::vec3& position) 
	: m_Position(position)
{

}

void Sprite::SetTexture(SDL_Renderer* renderer, const std::string& path)
{
    int key = m_StringHash(path);
    if (m_TextureAtlas.find(key) == m_TextureAtlas.end()) {

        Texture temp(path.c_str());
        m_TextureAtlas[key] = temp;
    }
    m_TextureKey = key;

    Texture& temp = m_TextureAtlas[m_TextureKey];
    m_SpriteHeight = temp.GetHeight();
    m_SpriteWidth = temp.GetWidth();

}

void Sprite::Update()
{
}
 
void Sprite::Draw(const Renderer& renderer, const Camera& camera, uint32_t* pixels, float* zBuffer) const
{

    //thanks to lodev for help with this!

    const Texture& tex = m_TextureAtlas[m_TextureKey];

	glm::vec2 cameraRight = glm::normalize(glm::vec2(-glm::sin(glm::radians(camera.Yaw)), glm::cos(glm::radians(camera.Yaw))));

	float spriteX = m_Position.x - camera.Position.x;
	float spriteY = m_Position.y - camera.Position.y;

	float dirX = glm::cos(glm::radians(camera.Yaw));
	float dirY = glm::sin(glm::radians(camera.Yaw));

	float planeX = cameraRight.x / 2.0f;
	float planeY = cameraRight.y / 2.0f;

	float invDet = 1.0f / (planeX * dirY - dirX * planeY);

	float transformX = invDet * (dirY * spriteX - dirX * spriteY);
	float transformY = invDet * (-planeY * spriteX + planeX * spriteY);

	if (transformY < 0 || -FALLOFF * transformY + 1 <= 0)
		return;

    int move = glm::round(m_Height / transformY) + camera.Pitch;

	int spriteScreenX = glm::round((renderer.GetWidth() / 2.0f) * (1 + transformX / transformY));

    int spriteHeight = glm::round(glm::abs((int)(renderer.GetHeight() / (transformY))) / m_Scale);
 
    int drawStartY = glm::round(-spriteHeight / 2.0f + renderer.GetHeight() / 2.0f + move);
    if (drawStartY < 0) drawStartY = 0;
    int drawEndY = glm::round(spriteHeight / 2.0f + renderer.GetHeight() / 2.0f + move);
    if (drawEndY >= renderer.GetHeight()) drawEndY = renderer.GetHeight() - 1;

 
    int spriteWidth = glm::round(glm::abs((int)(renderer.GetHeight() / (transformY))) / m_Scale);
    int drawStartX = -spriteWidth / 2.0f + spriteScreenX;
    int drawEndX = spriteWidth / 2 + spriteScreenX;
 
    for (int stripe = drawStartX; stripe < drawEndX; stripe++)
    {
        int texX = glm::round(glm::round(256.0f * (stripe - (-spriteWidth / 2.0f + spriteScreenX)) * tex.GetWidth() / spriteWidth) / 256.0f);
 
        if (transformY > 0 && stripe > 0 && stripe < renderer.GetWidth() && transformY < zBuffer[stripe]) {
            for (int y = (drawStartY < 0 ? 0 : drawStartY); y < (drawEndY >= renderer.GetHeight() ? renderer.GetHeight() : drawEndY); y++) 
            {
                int d = (y - move) * 256 - renderer.GetHeight() * 128 + spriteHeight * 128;  
                int texY = ((d * tex.GetHeight()) / spriteHeight) / 256;

                uint32_t color = tex.PixelAt(texX, texY);
                color = Util::MultiplyRGBA(color, -FALLOFF * transformY + 1);
                if ((color & 0xff) != 0)
                    pixels[y * renderer.GetWidth() + stripe] = color;
 
            }
        }
    }



}

Sprite::~Sprite()
{
}