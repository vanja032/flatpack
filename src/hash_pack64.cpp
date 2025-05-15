// hash_pack64.cpp
#include "flatpack/hash_pack64.hpp"
#include "flatpack/simd/simd.hpp" // SIMD

#include <iostream>

#define likely(x) __builtin_expect(!!(x), 1)   // Likely met condition
#define unlikely(x) __builtin_expect(!!(x), 0) // Unlikely met condition

namespace flatpack {

template <typename KeyT, typename ValueT>
HashPack64<KeyT, ValueT>::HashPack64(size_t initial_capacity)
    : capacity(initial_capacity), size(0) {
  flat_map = new Entry64[capacity];
  fingerprints_flat_array =
      static_cast<uint8_t *>(aligned_alloc(SIMD_WIDTH, capacity));
  for (size_t i = 0; i < capacity; ++i) {
    flat_map[i].key = EMPTY_KEY;
    flat_map[i].fingerprint = EMPTY_FINGERPRINT;
    fingerprints_flat_array[i] = EMPTY_FINGERPRINT;
  }
}

template <typename KeyT, typename ValueT>
HashPack64<KeyT, ValueT>::~HashPack64() noexcept {
  delete[] flat_map;
  delete[] fingerprints_flat_array;
}

template <typename KeyT, typename ValueT>
void HashPack64<KeyT, ValueT>::insert(const KeyT &key, ValueT value) {
  uint64_t packed_key = finalize_hash(key.value());
  uint8_t fingerprint = static_cast<uint8_t>(packed_key);
  size_t ideal_slot = hash_index(packed_key);
  size_t index = ideal_slot;
  size_t probe_distance = 0;

  Entry64 entry = {packed_key, value, fingerprint,
                   static_cast<uint16_t>(probe_distance)};

  while (true) {
    uint64_t current_key = flat_map[index].key;

    if (current_key == EMPTY_KEY) {
      flat_map[index] = entry;
      fingerprints_flat_array[index] = entry.fingerprint;
      ++size;
      return;
    }

    if (current_key == packed_key) {
      flat_map[index].value = value;
      return;
    }

    if (flat_map[index].probe_distance <
        entry
            .probe_distance) { // Robin Hood algo for insert with probe distance
      std::swap(entry, flat_map[index]);
      fingerprints_flat_array[index] = flat_map[index].fingerprint;
    }

    index = (index + 1) & (capacity - 1);
    ++entry.probe_distance;
  }
}

template <typename KeyT, typename ValueT>
bool HashPack64<KeyT, ValueT>::find(const KeyT &key, ValueT &value_out) {
  uint64_t packed_key = finalize_hash(key.value());
  uint8_t fingerprint = static_cast<uint8_t>(packed_key);
  size_t ideal_slot = hash_index(packed_key);
  size_t index = ideal_slot;
  size_t probe_distance = 0;

  if (likely(fingerprints_flat_array[index] == fingerprint &&
             flat_map[index].key == packed_key)) {
    value_out = flat_map[index].value;
    return true;
  }

  size_t next_index = (index + 1) & (capacity - 1);
  if (likely(fingerprints_flat_array[next_index] == fingerprint &&
             flat_map[next_index].key == packed_key)) {
    value_out = flat_map[next_index].value;
    return true;
  }

  while (true) {
    // Empty key -> unlikely
    if (unlikely(flat_map[index].key == EMPTY_KEY)) {
      return false;
    }

    // Robin Hood -> unlikely
    if (unlikely(
            flat_map[index].probe_distance <
            probe_distance)) { // Robin Hood algo for find with probe distance
      return false;
    }

    size_t remaining = capacity - index;
    if (remaining >= SIMD_WIDTH) {
      uint32_t mask = flatpack::simd::fingerprint_match_mask(
          &fingerprints_flat_array[index], fingerprint);

      for (; mask != 0; mask &= mask - 1) {
        int bit_index = __builtin_ctz(mask);
        size_t real_index = (index + bit_index) & (capacity - 1);

        if (flat_map[real_index].key == packed_key) {
          value_out = flat_map[real_index].value;
          return true;
        }
      }

      __builtin_prefetch(
          &fingerprints_flat_array[(index + SIMD_WIDTH) & (capacity - 1)], 0,
          1);
      __builtin_prefetch(&flat_map[(index + SIMD_WIDTH) & (capacity - 1)], 0,
                         1);

      index = (index + SIMD_WIDTH) & (capacity - 1);
      probe_distance += SIMD_WIDTH;
    } else {
      // Match → likely
      if (likely(fingerprints_flat_array[index] == fingerprint &&
                 flat_map[index].key == packed_key)) {
        value_out = flat_map[index].value;
        return true;
      }

      index = (index + 1) & (capacity - 1);
      ++probe_distance;
    }
  }
}

} // namespace flatpack

template class flatpack::HashPack64<flatpack::Key64, uint64_t>;
