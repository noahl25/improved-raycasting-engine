#pragma once

#include <glm/glm.hpp>

struct Camera {
	glm::vec2 Position = glm::vec2(0.0f, 0.0f);
	float Yaw = 0.0f;
	float Pitch = 0.0f;
	const float Height = 32.0f;
};