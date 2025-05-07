// simd_scallar.hpp
#pragma once
#include <cstdint>

#define SIMD_WIDTH 8

namespace flatpack {
namespace simd {

inline uint32_t fingerprint_match_mask(const uint8_t *ptr,
                                       uint8_t fingerprint) {
  uint32_t mask = 0;

  if (ptr[0] == fingerprint)
    mask |= (1u << 0);
  if (ptr[1] == fingerprint)
    mask |= (1u << 1);
  if (ptr[2] == fingerprint)
    mask |= (1u << 2);
  if (ptr[3] == fingerprint)
    mask |= (1u << 3);
  if (ptr[4] == fingerprint)
    mask |= (1u << 4);
  if (ptr[5] == fingerprint)
    mask |= (1u << 5);
  if (ptr[6] == fingerprint)
    mask |= (1u << 6);
  if (ptr[7] == fingerprint)
    mask |= (1u << 7);

  return mask;
}

} // namespace simd
} // namespace flatpack