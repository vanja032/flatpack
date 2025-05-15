// hash_pack64.hpp
#pragma once

#include "entry64.hpp"
#include "key64.hpp"
#include <cstddef>
#include <cstdint>

namespace flatpack {

template <typename KeyT = Key64, typename ValueT = uint64_t> class HashPack64 {
private:
  static constexpr uint64_t EMPTY_KEY = 0;
  static constexpr uint64_t EMPTY_FINGERPRINT = 0;

  Entry64 *flat_map;
  uint8_t *fingerprints_flat_array;
  size_t capacity;

public:
  HashPack64(size_t initial_capacity = 8192);
  ~HashPack64() noexcept;

  HashPack64(const HashPack64 &) = delete;
  HashPack64 &operator=(const HashPack64 &) = delete;

  void insert(const KeyT &key, ValueT value);
  bool find(const KeyT &key, ValueT &value_out);

private:
  inline size_t hash_index(uint64_t key_hash) const noexcept {
    return key_hash & (capacity - 1);
  }

  inline uint64_t finalize_hash(
      uint64_t key) const noexcept { // SplitMix64 inspired finalizing key hash
    key += 0x9e3779b97f4a7c15ULL;
    key = (key ^ (key >> 30)) * 0xbf58476d1ce4e5b9ULL;
    key = (key ^ (key >> 27)) * 0x94d049bb133111ebULL;
    key = key ^ (key >> 31);
    return key;
  }

public:
  size_t size;
};

} // namespace flatpack