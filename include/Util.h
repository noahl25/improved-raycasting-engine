#pragma once

#include <cstdint>
#include <cmath>
#include <glm/glm.hpp>
#include <unordered_map>
#include <SDL.h>

#include "Util.h"
 
#define EXPAND_UINT32(uint32) \
    ((uint8_t)((uint32) >> 24)), ((uint8_t)((uint32) >> 16)), ((uint8_t)((uint32) >> 8)), ((uint8_t)((uint32) >> 0))

#define DO_ONCE(code) \
    { static bool executed = false; if (!executed) { code; executed = true; } } 

#define FIELD(storage, field) &storage::field

namespace Util {

    template <typename First, typename Second>
    static constexpr bool is_different_v = !std::is_same_v<First, Second>;

	uint32_t AsUInt32(const glm::vec4& color);
	uint32_t AsUInt32(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	uint32_t AGBRtoRGBA(uint32_t ABGR);

    SDL_Rect PadRect(const SDL_Rect& rect, int pad);

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

    inline uint8_t Lerp(uint8_t a, uint8_t b, float t) {
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

    inline uint32_t Blend(uint32_t first, uint32_t second, float amount = 0.5f) {
        uint8_t* components1 = reinterpret_cast<uint8_t*>(&first);
        uint8_t* components2 = reinterpret_cast<uint8_t*>(&second);

        uint32_t result;
        uint8_t* resultComponents = reinterpret_cast<uint8_t*>(&result);

        for (int i = 0; i < 4; i++) {
            resultComponents[i] = Util::Lerp(components1[i], components2[i], amount);
        }

        return result;
    }
}