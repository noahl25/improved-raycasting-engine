#pragma once

#include <cstdint>
#include <cmath>
#include <glm/glm.hpp>
#include <unordered_map>
#include <immintrin.h>
 
#define EXPAND_UINT32(uint32) \
    ((uint8_t)((uint32) >> 24)), ((uint8_t)((uint32) >> 16)), ((uint8_t)((uint32) >> 8)), ((uint8_t)((uint32) >> 0))

namespace Util {
	uint32_t AsUInt32(const glm::vec4& color);
	uint32_t AsUInt32(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	uint32_t AGBRtoRGBA(uint32_t ABGR);
	void memcpy_reverse(void* dest, const void* src, int size);

    inline float Lerp(float a, float b, float t) {
        
        return a + (b - a) * t;
    }
 
    inline uint32_t MultiplyRGBA(uint32_t RGBA, float value) {

        if (value <= 0)
            return 0x000000ff;

        uint32_t r = (RGBA & 0xff000000) * value;
        uint32_t g = (RGBA & 0x00ff0000) * value;
        uint32_t b = (RGBA & 0x0000ff00) * value;
        uint32_t a = (RGBA & 0x000000ff);

        return (r & 0xff000000) | (g & 0x00ff0000) | (b & 0x0000ff00) | a;
    }
}