#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

#include "../include/flatpack/hash_pack64.hpp"

#define COUNT 7000

int main() {
  flatpack::HashPack64<flatpack::Key64, uint64_t> map;

  // Test with string key
  flatpack::Key64 key_str("testkey");
  map.insert(key_str, 12345);

  uint64_t result = 0;
  if (map.find(key_str, result)) {
    std::cout << "[PASS] String key test. Value: " << result << std::endl;
  } else {
    std::cout << "[FAIL] String key test." << std::endl;
  }

  // Test with packed integer key
  flatpack::Key64 key_int(987654321);
  map.insert(key_int, 67890);

  if (map.find(key_int, result)) {
    std::cout << "[PASS] Packed integer key test. Value: " << result
              << std::endl;
  } else {
    std::cout << "[FAIL] Packed integer key test." << std::endl;
  }

  // Test missing key
  flatpack::Key64 missing_key("missing");
  if (!map.find(missing_key, result)) {
    std::cout << "[PASS] Missing key test." << std::endl;
  } else {
    std::cout << "[FAIL] Missing key test. Found value: " << result
              << std::endl;
  }

  // Test bulk insert of 2000 keys
  bool bulk_pass = true;
  for (uint64_t i = 0; i < COUNT; ++i) {
    flatpack::Key64 key_bulk(std::string("key") + std::to_string(i));
    map.insert(key_bulk, i);
  }
  std::cout << "Map size: " << map.size << std::endl;
  for (uint64_t i = 0; i < COUNT; ++i) {
    flatpack::Key64 key_bulk(std::string("key") + std::to_string(i));
    uint64_t result_bulk = 0;
    if (!map.find(key_bulk, result_bulk) || result_bulk != i) {
      std::cout << "[FAIL] Bulk insert/find test for key" << i
                << " Value=" << result_bulk << std::endl;
      bulk_pass = false;
      // break;
    }
  }
  if (bulk_pass) {
    std::cout << "[PASS] Bulk insert/find test for " << COUNT << " keys."
              << std::endl;
  }

  uint64_t test_result = 0;
  flatpack::Key64 test_key("key" + std::to_string(122));
  map.find(test_key, test_result);
  std::cout << "Element on key:key" << 122 << " key_value=" << test_key.value()
            << ": " << test_result << std::endl;

  // ---- Measure insert time for HashPack64 ----
  auto start_insert_flatpack = std::chrono::high_resolution_clock::now();
  for (uint64_t i = 0; i < COUNT; ++i) {
    flatpack::Key64 key_bulk(std::string("key") + std::to_string(i));
    map.insert(key_bulk, i);
  }
  auto end_insert_flatpack = std::chrono::high_resolution_clock::now();
  auto insert_duration_flatpack =
      std::chrono::duration_cast<std::chrono::microseconds>(
          end_insert_flatpack - start_insert_flatpack)
          .count();

  std::cout << "[FLATPACK] Inserted " << COUNT << " keys in "
            << insert_duration_flatpack << " microseconds." << std::endl;

  // ---- Measure find time for HashPack64 ----
  auto start_find_flatpack = std::chrono::high_resolution_clock::now();
  bool flatpack_bulk_pass = true;
  for (uint64_t i = 0; i < COUNT; ++i) {
    flatpack::Key64 key_bulk(std::string("key") + std::to_string(i));
    uint64_t result_bulk = 0;
    if (!map.find(key_bulk, result_bulk) || result_bulk != i) {
      std::cout << "[FAIL] Flatpack find failed for key " << i << std::endl;
      flatpack_bulk_pass = false;
      break;
    }
  }
  auto end_find_flatpack = std::chrono::high_resolution_clock::now();
  auto find_duration_flatpack =
      std::chrono::duration_cast<std::chrono::microseconds>(end_find_flatpack -
                                                            start_find_flatpack)
          .count();

  if (flatpack_bulk_pass) {
    std::cout << "[FLATPACK] Found " << COUNT << " keys in "
              << find_duration_flatpack << " microseconds." << std::endl;
  }

  // ---- Measure insert time for std::unordered_map ----
  std::unordered_map<std::string, uint64_t> std_map;

  auto start_insert_std = std::chrono::high_resolution_clock::now();
  for (uint64_t i = 0; i < COUNT; ++i) {
    std_map["key" + std::to_string(i)] = i * 10;
  }
  auto end_insert_std = std::chrono::high_resolution_clock::now();
  auto insert_duration_std =
      std::chrono::duration_cast<std::chrono::microseconds>(end_insert_std -
                                                            start_insert_std)
          .count();

  std::cout << "[STD MAP] Inserted " << COUNT << " keys in "
            << insert_duration_std << " microseconds." << std::endl;

  // ---- Measure find time for std::unordered_map ----
  auto start_find_std = std::chrono::high_resolution_clock::now();
  bool std_bulk_pass = true;
  for (uint64_t i = 0; i < COUNT; ++i) {
    auto it = std_map.find("key" + std::to_string(i));
    if (it == std_map.end() || it->second != i * 10) {
      std::cout << "[FAIL] Std map find failed for key " << i << std::endl;
      std_bulk_pass = false;
      break;
    }
  }
  auto end_find_std = std::chrono::high_resolution_clock::now();
  auto find_duration_std =
      std::chrono::duration_cast<std::chrono::microseconds>(end_find_std -
                                                            start_find_std)
          .count();

  if (std_bulk_pass) {
    std::cout << "[STD MAP] Found " << COUNT << " keys in " << find_duration_std
              << " microseconds." << std::endl;
  }

  return 0;
}