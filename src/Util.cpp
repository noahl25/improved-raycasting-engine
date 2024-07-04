#include "Util.h"

namespace Util {

    uint32_t AsUInt32(const glm::vec4& color)
    {
        return (((uint8_t)(color.r * 255.0f)) << 24) | (((uint8_t)(color.g * 255.0f)) << 16) | (((uint8_t)(color.b * 255.0f)) << 8) | (uint8_t)(color.a * 255.0f);
    }

    uint32_t AsUInt32(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        uint32_t color = 0;
        color |= (r << 24);
        color |= (g << 16);
        color |= (b << 8);
        color |= a;
        return color;
    }


    uint32_t AGBRtoRGBA(uint32_t abgr) {
 
        uint8_t a = (abgr >> 24) & 0xFF;
        uint8_t b = (abgr >> 16) & 0xFF;
        uint8_t g = (abgr >> 8) & 0xFF;
        uint8_t r = abgr & 0xFF;

 
        return (r << 24) | (g << 16) | (b << 8) | a;
 
    }

    void memcpy_reverse(void* dest, const void* src, int size)
    {
        unsigned char* d = (unsigned char*)dest;
        const unsigned char* s = (const unsigned char*)src;

        for (size_t i = 0; i < size; i++) {
            d[i] = s[size - 1 - i];
        }
    }
    

};