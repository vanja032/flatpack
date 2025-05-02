// entry64.hpp
#pragma once

#include <cstdint>

namespace flatpack {

struct Entry64 {          // Padded to 24 bytes anyway by Compiler
  uint64_t key;           // 8 bytes
  uint64_t value;         // 8 bytes
  uint8_t fingerprint;    // 1 byte => Fast compare
  uint8_t probe_distance; // 1 byte => Robin Hood algo
  uint8_t reserved;       // 1 byte
                          // = 19 bytes > 16 bytes
};

} // namespace flatpack