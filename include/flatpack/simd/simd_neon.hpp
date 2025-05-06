// simd_neon.hpp
#pragma once
#include <arm_neon.h>

#define SIMD_WIDTH 16

namespace flatpack {
namespace simd {

inline uint32_t fingerprint_match_mask(const uint8_t *ptr,
                                       uint8_t fingerprint) {
  uint8x16_t fingerprints = vld1q_u8(ptr);
  uint8x16_t target = vdupq_n_u8(fingerprint);
  uint8x16_t comps = vceqq_u8(fingerprints, target);

  uint8_t result[16];
  vst1q_u8(result, comps);

  uint32_t mask = 0;
  for (int i = 0; i < 16; ++i) {
    if (result[i])
      mask |= (1u << i);
  }
  return mask;
}

} // namespace simd
} // namespace flatpack