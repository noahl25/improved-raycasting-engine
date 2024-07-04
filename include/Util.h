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

	__forceinline uint32_t MultiplyRGBA(uint32_t RGBA, float value) {
 
        __m128i color = _mm_set1_epi32(RGBA);
 
        __m128i mask = _mm_set1_epi32(0xFF);
        __m128i r = _mm_and_si128(_mm_srli_epi32(color, 24), mask);
        __m128i g = _mm_and_si128(_mm_srli_epi32(color, 16), mask);
        __m128i b = _mm_and_si128(_mm_srli_epi32(color, 8), mask);
        __m128i a = _mm_and_si128(color, mask);

        __m128 fr = _mm_cvtepi32_ps(r);
        __m128 fg = _mm_cvtepi32_ps(g);
        __m128 fb = _mm_cvtepi32_ps(b);
        __m128 fa = _mm_cvtepi32_ps(a);

        __m128 factor = _mm_set1_ps(value);
        fr = _mm_mul_ps(fr, factor);
        fg = _mm_mul_ps(fg, factor);
        fb = _mm_mul_ps(fb, factor);
        fa = _mm_mul_ps(fa, factor);

        __m128i ir = _mm_min_epi32(_mm_max_epi32(_mm_cvtps_epi32(fr), _mm_setzero_si128()), mask);
        __m128i ig = _mm_min_epi32(_mm_max_epi32(_mm_cvtps_epi32(fg), _mm_setzero_si128()), mask);
        __m128i ib = _mm_min_epi32(_mm_max_epi32(_mm_cvtps_epi32(fb), _mm_setzero_si128()), mask);
        __m128i ia = _mm_min_epi32(_mm_max_epi32(_mm_cvtps_epi32(fa), _mm_setzero_si128()), mask);

        __m128i result = _mm_or_si128(_mm_or_si128(_mm_slli_epi32(ir, 24), _mm_slli_epi32(ig, 16)), _mm_or_si128(_mm_slli_epi32(ib, 8), ia));

        uint32_t finalColor;
        _mm_store_ss(reinterpret_cast<float*>(&finalColor), _mm_castsi128_ps(result));
        return finalColor;
	}
}