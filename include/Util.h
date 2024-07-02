#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#define EXPAND_UINT32(uint32) \
    ((uint8_t)((uint32) >> 24)), ((uint8_t)((uint32) >> 16)), ((uint8_t)((uint32) >> 8)), ((uint8_t)((uint32) >> 0))

namespace Util {
	uint32_t AsUInt32(const glm::vec4& color);
	uint32_t AsUInt32(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	uint32_t AGBRtoRGBA(uint32_t ABGR);
	void memcpy_reverse(void* dest, const void* src, int size);

	__forceinline uint32_t MultiplyRGBA(uint32_t RGBA, float value) {
        uint8_t r = (RGBA >> 24) & 0xFF;
        uint8_t g = (RGBA >> 16) & 0xFF;
        uint8_t b = (RGBA >> 8) & 0xFF;
        uint8_t a = RGBA & 0xFF;

        r = (uint8_t)glm::clamp(r * value, 0.0f, 255.0f);
        g = (uint8_t)glm::clamp(g * value, 0.0f, 255.0f);
        b = (uint8_t)glm::clamp(b * value, 0.0f, 255.0f);
        a = (uint8_t)glm::clamp(a * value, 0.0f, 255.0f);

        return (r << 24) | (g << 16) | (b << 8) | a;
	}
};