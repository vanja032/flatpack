// key64.hpp
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace flatpack {

/**
 * @brief Packs a human-readable string into a compact 64-bit representation.
 *
 * Key64 encodes up to 12 characters using a 5-bit encoding scheme optimized
 * for:
 * - '_' (underscore)
 * - Lowercase letters ('a' to 'z')
 * - Digits ('1' to '5')
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

  static inline constexpr std::array<uint8_t, 256> symbol_table = [] {
    std::array<uint8_t, 256> table{};

    table[static_cast<uint8_t>('_')] = 0;
    for (char c = 'a'; c <= 'z'; ++c) {
      table[static_cast<uint8_t>(c)] = c - 'a' + 1;
    }
    for (char c = '1'; c <= '5'; ++c) {
      table[static_cast<uint8_t>(c)] = c - '1' + 27;
    }

    return table;
  }();

public:
  Key64(uint64_t packed) : packed_key(packed) {}

  Key64(const char *key) {
    packed_key = 0;
    for (size_t i = 0; i < 12 && key[i] != '\0'; ++i) {
      packed_key <<= 5;
      packed_key |= encode_symbol(key[i]);
    }
    size_t length = std::strlen(key);
    size_t remaining = 12 - length;
    packed_key <<= (remaining * 5);
    packed_key <<= 4;
    packed_key |= length;
  }

  ~Key64() = default;

  [[nodiscard]] uint64_t value() const noexcept { return packed_key; }

  bool operator==(const Key64 &other) const noexcept {
    return packed_key == other.packed_key;
  }

private:
  /**
   * @brief Encodes a symbol into a 5-bit representation.
   *
   * Encoding rules:
   * - The '_' (underscore) character is encoded as 0b00000 (value 0).
   * - Lowercase letters 'a' to 'z' are encoded as 0b00001 to 0b11010 (values 1
   * to 26).
   *   - Example: 'a' → 1 (0b00001), 'b' → 2 (0b00010), ..., 'z' → 26 (0b11010).
   * - Digits '1' to '5' are encoded as 0b11011 to 0b11111 (values 27 to 31).
   *   - Example: '1' → 27 (0b11011), '2' → 28 (0b11100), ..., '5' → 31
   * (0b11111).
   * - Any other characters are considered invalid and are encoded as 0b00000
   * (value 0), same as the underscore.
   *
   * This encoding ensures that every valid character is represented in exactly
   * 5 bits, and the entire key can be packed into a 64-bit integer along with
   * its length.
   *
   * @param symbol The character to encode.
   * @return uint8_t Encoded 5-bit representation.
   */
  static constexpr uint8_t encode_symbol(char symbol) noexcept {
    return symbol_table[static_cast<uint8_t>(symbol)];
  }
};

} // namespace flatpack