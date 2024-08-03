#pragma once

#include <vector>
#include <filesystem>

#include "Sprite.h"
#include "Animation.h"

class AnimatedSprite : public Sprite {

public:

	virtual void Update() override;
	void SetTexture(SDL_Renderer* renderer, const std::string& pathToDir);

	inline void SetAnimationSpeed(int speed) { m_AnimationSpeed = speed; }

private:

	std::vector<int> m_FrameKeys;
	int m_CurrentFrame = 0;

	int m_AnimationSpeed = 1;

};