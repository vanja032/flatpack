// hash_pack64.cpp
#include "flatpack/hash_pack64.hpp"
#include "flatpack/simd/simd.hpp" // SIMD

#include <iostream>

namespace flatpack {

HashPack64::HashPack64(size_t initial_capacity)
    : capacity(initial_capacity), size(0) {
  flat_map = new Entry64[capacity];
  fingerprints_flat_array = new uint8_t[capacity];
  for (size_t i = 0; i < capacity; ++i) {
    flat_map[i].key = EMPTY_KEY;
    flat_map[i].fingerprint = EMPTY_FINGERPRINT;
    fingerprints_flat_array[i] = EMPTY_FINGERPRINT;
  }
}

HashPack64::~HashPack64() noexcept { delete[] flat_map; }

void HashPack64::insert(const Key64 &key, uint64_t value) {
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
      // std::cout << "INSERT: " << key.get_original_key() << "=" << key.value()
      //           << " fingerprint=" << (int)fingerprint
      //           << " At position: " << index
      //           << " and Probe=" << entry.probe_distance << std::endl;
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

bool HashPack64::find(const Key64 &key, uint64_t &value_out) {
  uint64_t packed_key = finalize_hash(key.value());
  uint8_t fingerprint = static_cast<uint8_t>(packed_key);
  size_t ideal_slot = hash_index(packed_key);
  size_t index = ideal_slot;
  size_t probe_distance = 0;

  while (true) {
    // if (flat_map[index].key == packed_key) {
    //   value_out = flat_map[index].value;
    //   return true;
    // }

    if (flat_map[index].key == EMPTY_KEY) {
      return false;
    }

    if (flat_map[index].probe_distance <
        probe_distance) { // Robin Hood algo for find with probe distance
      // std::cout << "FIND: " << key.get_original_key() << "=" << key.value()
      //           << " fingerprint=" << (int)fingerprint
      //           << " probe=" << flat_map[index].probe_distance << "|"
      //           << probe_distance << std::endl;
      return false;
    }

    size_t remaining = capacity - index;
    if (remaining >= SIMD_WIDTH) {
      uint32_t mask = flatpack::simd::fingerprint_match_mask(
          &fingerprints_flat_array[index], fingerprint);

      while (mask != 0) {
        int bit_index = __builtin_ctz(mask);
        size_t real_index = (index + bit_index) & (capacity - 1);

        // std::cout << "MASK: " << mask << " | BIT INDEX: " << bit_index << " |
        // "
        //           << key.get_original_key() << " | " <<
        //           flat_map[real_index].key
        //           << "==" << packed_key
        //           << " | Value=" << flat_map[real_index].value
        //           << " | Searched Fingerprint=" << (int)fingerprint
        //           << " | Slots Fingerprint="
        //           << (int)flat_map[real_index].fingerprint
        //           << " | At Position: " << real_index << std::endl;
        if (flat_map[real_index].key == packed_key) {
          value_out = flat_map[real_index].value;
          // std::cout << "FIND: " << key.get_original_key() << "=" <<
          // key.value()
          //           << " fingerprint=" << (int)fingerprint << " mask=" <<
          //           mask
          //           << std::endl;
          return true;
        }
        mask &= mask - 1;
      }

      index = (index + SIMD_WIDTH) & (capacity - 1);
      probe_distance += SIMD_WIDTH;
    } else {
      if (flat_map[index].fingerprint == fingerprint &&
          flat_map[index].key == packed_key) {
        value_out = flat_map[index].value;
        return true;
      }

      index = (index + 1) & (capacity - 1);
      ++probe_distance;
    }
  }
}

} // namespace flatpack
