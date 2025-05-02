// hash_pack64.cpp
#include "flatpack/hash_pack64.hpp"

namespace flatpack {

HashPack64::HashPack64(size_t initial_capacity)
    : capacity(initial_capacity), size(0) {
  flat_map = new Entry64[capacity];
  for (size_t i = 0; i < capacity; ++i) {
    flat_map[i].key = EMPTY_KEY;
  }
}

HashPack64::~HashPack64() noexcept { delete[] flat_map; }

void HashPack64::insert(const Key64 &key, uint64_t value) {
  uint64_t packed_key = key.value();
  uint8_t fingerprint = static_cast<uint8_t>(packed_key);
  size_t ideal_slot = hash_index(packed_key);
  size_t index = ideal_slot;
  size_t probe_distance = 0;

  Entry64 entry = {packed_key, value, fingerprint,
                   static_cast<uint8_t>(probe_distance)};

  while (true) {
    uint64_t current_key = flat_map[index].key;

    if (current_key == EMPTY_KEY) {
      flat_map[index] = entry;
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
    }

    index = (index + 1) & (capacity - 1);
    ++entry.probe_distance;
  }
}

bool HashPack64::find(const Key64 &key, uint64_t &value_out) {
  uint64_t packed_key = key.value();
  uint8_t fingerprint = static_cast<uint8_t>(packed_key);
  size_t ideal_slot = hash_index(packed_key);
  size_t index = ideal_slot;
  size_t probe_distance = 0;

  while (true) {
    uint64_t current_key = flat_map[index].key;

    if (current_key == EMPTY_KEY) {
      return false;
    }

    if (flat_map[index].probe_distance <
        probe_distance) { // Robin Hood algo for find with probe distance
      return false;
    }

    if (flat_map[index].fingerprint == fingerprint &&
        flat_map[index].key == packed_key) {
      value_out = flat_map[index].value;
      return true;
    }

    index = (index + 1) & (capacity - 1);
    ++probe_distance;
  }
}

} // namespace flatpack
