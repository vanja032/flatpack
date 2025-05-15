# Flatpack

Flatpack is a lightweight and high-performance C++ library designed for fast and memory-efficient data structures.  
Currently, it offers an advanced Robin Hood hash map implementation optimized for scenarios where speed and cache locality matter.  
The design focuses on simplicity, performance, and minimal external dependencies.

## Features

- Robin Hood hashing with optimized probing strategy
- Flat layout for better cache locality
- **64-bit** packed keys: **60 bits** store encoded key data, **4 bits** store the true key length, and the lowest 8 bits are used as a fingerprint
- Fast lookup using fingerprint scan (SCALAR, AVX2, SSE2, ARM NEON SIMD optimization)
- Dynamic resizing
- Header-only design (easy integration)

## Usage

### Build

This library uses CMake for building.

#### There are two ways to build Flatpack:

#### 1. Using CMake manually

```bash
mkdir build
cd build
cmake ..
make
```

#### 2. Using the provided build script

```bash
chmod +x build.sh
./build.sh
```

The build script will automatically create the build directory and compile the project.

Or simply include the header files directly into your project.

## Key Packing Algorithm

Flatpack provides utility functions to encode strings (such as `_a-z1-9`) into compact 64-bit keys.  
The packed format reserves 60 bits to store the encoded key, 4 bits to indicate the true key length, and uses the lowest 8 bits of the key as a fingerprint for fast lookups.

## Roadmap

- Reshape (resize) support for hash map (rehashing and growing/shrinking the map)
- Refactor of internal layout for improved memory usage and insertion speed
- Additional data structures (flat set, fixed-size map)
- Benchmarks against standard containers
- Thread-safety utilities

## Benchmark Results

**Test Environment:**

- Device: Apple Mac with M2 chip
- RAM: 16 GB
- OS: macOS

Performance comparison on inserting and finding 7000 keys:

- Best case (avg):

```
[FLATPACK] Inserted 7000 keys in 741 microseconds.
[FLATPACK] Found 7000 keys in 750 microseconds.
[STD MAP] Inserted 7000 keys in 1139 microseconds.
[STD MAP] Found 7000 keys in 717 microseconds.
```

- Worst case (avg):

```
[FLATPACK] Inserted 7000 keys in 630 microseconds.
[FLATPACK] Found 7000 keys in 707 microseconds.
[STD MAP] Inserted 7000 keys in 1045 microseconds.
[STD MAP] Found 7000 keys in 520 microseconds.
```

- Random case:

```
[FLATPACK] Inserted 7000 keys in 748 microseconds.
[FLATPACK] Found 7000 keys in 857 microseconds.
[STD MAP] Inserted 7000 keys in 1643 microseconds.
[STD MAP] Found 7000 keys in 950 microseconds.
```

Flatpack demonstrates significantly faster insert performance and competitive find performance compared to the standard `std::map`.

## Usage

### Basic Example

```cpp
#include <iostream>
#include "flatpack/hash_pack64.hpp"

int main() {
    flatpack::HashPack64 map;

    // Insert a string key
    flatpack::Key64 key_str("testkey");
    map.insert(key_str, 12345);

    // Lookup the string key
    uint64_t result = 0;
    if (map.find(key_str, result)) {
        std::cout << "Found string key with value: " << result << std::endl;
    }

    // Insert a packed integer key
    flatpack::Key64 key_int(987654321);
    map.insert(key_int, 67890);

    // Lookup the packed integer key
    if (map.find(key_int, result)) {
        std::cout << "Found integer key with value: " << result << std::endl;
    }

    return 0;
}
```

### Bulk Insert and Find

```cpp
#include <iostream>
#include "flatpack/hash_pack64.hpp"

#define COUNT 7000

int main() {
    flatpack::HashPack64 map;

    // Bulk insert
    for (uint64_t i = 0; i < COUNT; ++i) {
        flatpack::Key64 key("key" + std::to_string(i));
        map.insert(key, i);
    }

    std::cout << "Inserted " << COUNT << " keys." << std::endl;

    // Bulk find
    for (uint64_t i = 0; i < COUNT; ++i) {
        flatpack::Key64 key("key" + std::to_string(i));
        uint64_t result = 0;
        if (!map.find(key, result) || result != i) {
            std::cout << "Failed to find key: key" << i << std::endl;
        }
    }

    std::cout << "Verified all keys." << std::endl;

    return 0;
}
```

## License

This project is licensed under the MIT License.

## Author

Created and maintained by Vanja Sretenovic. For more information or contributions, visit [https://github.com/vanja032](https://github.com/vanja032).
