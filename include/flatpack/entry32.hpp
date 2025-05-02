// entry32.hpp
#pragma once

#include <cstdint>

namespace flatpack {

struct Entry32 {          // Padded to 8 bytes anyway by Compiler
  uint32_t key;           // 4 bytes
  uint16_t value;         // 2 bytes
  uint8_t fingerprint;    // 1 byte
  uint8_t probe_distance; // 1 byte
                          // = 8 bytes == 8 bytes
};

} // namespace flatpack