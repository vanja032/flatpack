// simd_avx2.hpp
#pragma once
#include <immintrin.h>

#define SIMD_WIDTH 32

namespace flatpack {
namespace simd {

inline uint32_t fingerprint_match_mask(const uint8_t *ptr,
                                       uint8_t fingerprint) {
  __m256i fingerprints =
      _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ptr));
  __m256i target = _mm256_set1_epi8(fingerprint);
  __m256i comps = _mm256_cmpeq_epi8(fingerprints, target);
  return _mm256_movemask_epi8(comps);
}

} // namespace simd
} // namespace flatpack