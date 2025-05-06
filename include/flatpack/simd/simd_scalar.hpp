// simd_scallar.hpp
#pragma once
#include <cstdint>

#define SIMD_WIDTH 8

namespace flatpack {
namespace simd {

inline uint32_t fingerprint_match_mask(const uint8_t *ptr,
                                       uint8_t fingerprint) {
  uint32_t mask = 0;
  for (int i = 0; i < SIMD_WIDTH; ++i) {
    if (ptr[i] == fingerprint)
      mask |= (1u << i);
  }
  return mask;
}

} // namespace simd
} // namespace flatpack