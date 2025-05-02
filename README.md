# Flatpack

Flatpack is a lightweight and high-performance C++ library designed for fast and memory-efficient data structures.  
Currently, it offers an advanced Robin Hood hash map implementation optimized for scenarios where speed and cache locality matter.  
The design focuses on simplicity, performance, and minimal external dependencies.

## Features

- Robin Hood hashing with optimized probing strategy
- Flat layout for better cache locality
- **64-bit** packed keys: **60 bits** store encoded key data, **4 bits** store the true key length, and the lowest 8 bits are used as a fingerprint
- Fast lookup using fingerprint scan (planned: AVX2 SIMD optimization)
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

Flatpack provides utility functions to encode strings (such as `_a-z1-5`) into compact 64-bit keys.  
The packed format reserves 60 bits to store the encoded key, 4 bits to indicate the true key length, and uses the lowest 8 bits of the key as a fingerprint for fast lookups.

## Roadmap

- AVX2 SIMD fingerprint scanning
- Reshape (resize) support for hash map (rehashing and growing/shrinking the map)
- Refactor of internal layout for improved memory usage and insertion speed
- Additional data structures (flat set, fixed-size map)
- Benchmarks against standard containers
- Thread-safety utilities

## License
