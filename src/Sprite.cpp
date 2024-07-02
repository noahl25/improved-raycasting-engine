#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(const glm::vec3& position) 
	: m_Position(position)
{

}

void Sprite::SetTexture(SDL_Renderer* renderer, const char* path)
{
    test.SetTexture(path);
}

void Sprite::Update()
{
}
 
void Sprite::Draw(Renderer& renderer, const Camera& camera, uint32_t* pixels, float* zBuffer) const
{

    if (glm::distance(camera.Position, glm::vec2(m_Position.x, m_Position.y)) > 5.0f)
        return;

	glm::vec2 cameraRight = glm::normalize(glm::vec2(-glm::sin(glm::radians(camera.Yaw)), glm::cos(glm::radians(camera.Yaw))));

	float spriteX = m_Position.x - camera.Position.x;
	float spriteY = m_Position.y - camera.Position.y;

	float dirX = glm::cos(glm::radians(camera.Yaw));
	float dirY = glm::sin(glm::radians(camera.Yaw));

	float planeX = cameraRight.x / 2;
	float planeY = cameraRight.y / 2;

	float invDet = 1.0f / (planeX * dirY - dirX * planeY);

	float transformX = invDet * (dirY * spriteX - dirX * spriteY);
	float transformY = invDet * (-planeY * spriteX + planeX * spriteY);

	if (transformY < 0)
		return;

	int spriteScreenX = (int)((renderer.GetWidth() / 2) * (1 + transformX / transformY));

    int spriteHeight = abs(int(renderer.GetHeight() / (transformY))); 
 
    int drawStartY = -spriteHeight / 2 + renderer.GetHeight() / 2;
    if (drawStartY < 0) drawStartY = 0;
    int drawEndY = spriteHeight / 2 + renderer.GetHeight() / 2;
    if (drawEndY >= renderer.GetHeight()) drawEndY = renderer.GetHeight() - 1;

 
    int spriteWidth = abs(int(renderer.GetHeight() / (transformY)));
    int drawStartX = -spriteWidth / 2 + spriteScreenX;
    if (drawStartX < 0) drawStartX = 0;
    int drawEndX = spriteWidth / 2 + spriteScreenX;
    if (drawEndX >= renderer.GetWidth()) drawEndX = renderer.GetWidth() - 1;
 
 
    for (int stripe = drawStartX; stripe < drawEndX; stripe++)
    {
        int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * test.GetWidth() / spriteWidth) / 256;
 
        if (transformY > 0 && stripe > 0 && stripe < renderer.GetWidth() && transformY < zBuffer[stripe]) {
            for (int y = drawStartY; y < drawEndY; y++) 
            {
                int d = (y) * 256 - renderer.GetHeight() * 128 + spriteHeight * 128;  
                int texY = ((d * test.GetHeight()) / spriteHeight) / 256;

                uint32_t color = test.PixelAtOptimized(texX, texY);
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