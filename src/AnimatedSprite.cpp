#include "AnimatedSprite.h"

void AnimatedSprite::Update()
{
	if (Animation::Counter() % m_AnimationSpeed == 0) {
		m_CurrentFrame++;
		if (m_CurrentFrame > m_FrameKeys.size() - 1)
			m_CurrentFrame = 0;
	}

	ChangeTexture(m_FrameKeys[m_CurrentFrame]);

}

void AnimatedSprite::SetTexture(SDL_Renderer* renderer, const std::string& pathToDir)
{
	for (const auto& img : std::filesystem::directory_iterator(pathToDir)) {
		int key = PreloadTexture(renderer, img.path().generic_string());
		m_FrameKeys.push_back(key);
		Texture& temp = m_TextureAtlas[key];
		m_SpriteHeight = temp.GetHeight();
		m_SpriteWidth = temp.GetWidth();
		m_ModificationTable = new uint32_t[m_SpriteWidth * m_SpriteHeight];
	}

}