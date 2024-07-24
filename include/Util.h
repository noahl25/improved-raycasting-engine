#pragma once

#include <cstdint>
#include <cmath>
#include <glm/glm.hpp>
#include <unordered_map>
 
#define EXPAND_UINT32(uint32) \
    ((uint8_t)((uint32) >> 24)), ((uint8_t)((uint32) >> 16)), ((uint8_t)((uint32) >> 8)), ((uint8_t)((uint32) >> 0))

#define DO_ONCE(code) \
    { static bool executed = false; if (!executed) { code; executed = true; } } 

#define FIELD(storage, field) &storage::field

namespace Util {
	uint32_t AsUInt32(const glm::vec4& color);
	uint32_t AsUInt32(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	uint32_t AGBRtoRGBA(uint32_t ABGR);

    template <typename First, typename Second>
    static constexpr bool is_different_v = !std::is_same_v<First, Second>;

    inline void memcpy_reverse(void* dest, const void* src, int size) {
        unsigned char* d = (unsigned char*)dest;
        const unsigned char* s = (const unsigned char*)src;

        for (size_t i = 0; i < size; i++) {
            d[i] = s[size - 1 - i];
        }
    }

    inline float Lerp(float a, float b, float t) {
        
        return a + (b - a) * t;
    }
 
    inline uint32_t MultiplyRGBA(uint32_t color, float value) {

        if (value < 0)
            return 0x0;

        uint8_t* components = reinterpret_cast<uint8_t*>(&color);
        components[1] = components[1] * value;
        components[2] = components[2] * value;
        components[3] = components[3] * value;

        return color;

    }

    inline void MultiplyRGBA(const uint32_t* src, uint32_t* dest, int size, float value) {

        if (value < 0) {
            std::memset(dest, 0, size * sizeof(uint32_t));
            return;
        }

        const uint8_t* components = reinterpret_cast<const uint8_t*>(src);
        uint8_t* componentsDest = reinterpret_cast<uint8_t*>(dest);

        for (int i = 0; i < size * 4; i++) {
            componentsDest[i] = components[i] * value;
        }

    }

    inline bool InRange(int number, int low, int high) {
        if ((unsigned)(number - low) <= high - low)
            return true;
        return false;
    }
}