// simd_sse2.hpp
#pragma once
#include <emmintrin.h>

#define SIMD_WIDTH 16

namespace flatpack {
namespace simd {

inline uint32_t fingerprint_match_mask(const uint8_t *ptr,
                                       uint8_t fingerprint) {
  __m128i fingerprints =
      _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr));
  __m128i target = _mm_set1_epi8(fingerprint);
  __m128i comps = _mm_cmpeq_epi8(fingerprints, target);
  return _mm_movemask_epi8(comps);
}

} // namespace simd
} // namespace flatpack