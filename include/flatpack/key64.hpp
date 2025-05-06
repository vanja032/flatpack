// key64.hpp
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace flatpack {

/**
 * @brief Packs a human-readable string into a compact 64-bit representation.
 *
 * Key64 encodes up to 10 characters using a 6-bit encoding scheme optimized
 * for:
 * - '_' (underscore)
 * - Lowercase letters ('a' to 'z')
 * - Digits ('0' to '9')
 *
 * The encoded string is packed into a 64-bit integer, with the last 4 bits
 * storing the original string length for precise comparison and decoding.
 *
 * This format allows efficient storage, fast equality checks, and is
 * well-suited for use as keys in hash maps, databases, or network protocols.
 */
class Key64 {
private:
  uint64_t packed_key;

  static inline std::array<uint8_t, 256> symbol_table = [] {
    std::array<uint8_t, 256> table{};

    table[static_cast<uint8_t>('_')] = 0;

    // a-z → 1 to 26
    for (char c = 'a'; c <= 'z'; ++c) {
      table[static_cast<uint8_t>(c)] = c - 'a' + 1;
    }

    // 0-9 → 27 to 36
    for (char c = '0'; c <= '9'; ++c) {
      table[static_cast<uint8_t>(c)] = c - '0' + 27;
    }

    return table;
  }();

public:
  Key64(uint64_t packed) : packed_key(packed) {}

  Key64(const char *key) {
    packed_key = 0;
    for (size_t i = 0; i < 10 && key[i] != '\0'; ++i) {
      packed_key <<= 6;
      packed_key |= encode_symbol(key[i]);
    }
    size_t length = std::strlen(key);
    size_t remaining = 10 - length;
    packed_key <<= (remaining * 6);
    packed_key <<= 4;
    packed_key |= length;
  }

  Key64(const std::string &key) : Key64(key.c_str()) {}

  ~Key64() = default;

  [[nodiscard]] uint64_t value() const noexcept { return packed_key; }

  bool operator==(const Key64 &other) const noexcept {
    return packed_key == other.packed_key;
  }

private:
  /**
   * @brief Encodes a symbol into a 6-bit representation.
   *
   * Encoding rules:
   * - The '_' (underscore) character is encoded as 0 (0b000000).
   * - Lowercase letters 'a' to 'z' are encoded as 1 to 26 (0b000001 to
   * 0b011010).
   *   - Example: 'a' → 1 (0b000001), 'b' → 2 (0b000010), ..., 'z' → 26
   * (0b011010).
   * - Digits '0' to '9' are encoded as 27 to 36 (0b011011 to 0b100100).
   *   - Example: '0' → 27 (0b011011), '1' → 28 (0b011100), ..., '9' → 36
   * (0b100100).
   * - Any other characters are considered invalid and encoded as 0 (same as the
   * underscore).
   *
   * This encoding ensures that every valid character is represented in exactly
   * 6 bits, and the entire key can be packed into a 64-bit integer along with
   * its length.
   *
   * @param symbol The character to encode.
   * @return uint8_t Encoded 6-bit representation.
   */
  static constexpr uint8_t encode_symbol(char symbol) noexcept {
    return symbol_table[static_cast<uint8_t>(symbol)];
  }
};

} // namespace flatpack