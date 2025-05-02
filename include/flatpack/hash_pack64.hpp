// hash_pack64.hpp
#pragma once

#include "entry64.hpp"
#include "key64.hpp"
#include <cstddef>
#include <cstdint>

namespace flatpack {

class HashPack64 {
private:
  static constexpr uint64_t EMPTY_KEY = 0;

  Entry64 *flat_map;
  size_t capacity;
  size_t size;

public:
  HashPack64(size_t initial_capacity = 4096);
  ~HashPack64();

  HashPack64(const HashPack64 &) = delete;
  HashPack64 &operator=(const HashPack64 &) = delete;

  void insert(const Key64 &key, uint64_t value);
  bool find(const Key64 &key, uint64_t &value_out);

private:
  inline size_t hash_index(uint64_t key_hash) const noexcept {
    return key_hash & (capacity - 1);
  }
};

} // namespace flatpack