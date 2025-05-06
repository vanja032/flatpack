cmake_minimum_required(VERSION 3.14)

project(flatpack
    VERSION 1.0.0
    DESCRIPTION "FlatPack - High performance Robin Hood hashmap"
    LANGUAGES CXX
)

# Extract version parts
set(FLATPACK_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(FLATPACK_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(FLATPACK_VERSION_PATCH ${PROJECT_VERSION_PATCH})

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add library target
add_library(flatpack STATIC
    src/hash_pack64.cpp
)

# Check CPU features
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
check_cxx_compiler_flag("-msse2" COMPILER_SUPPORTS_SSE2)

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
    #include <arm_neon.h>
    int main() {
        uint8x16_t a = vdupq_n_u8(0);
        (void)a;
        return 0;
    }
"    COMPILER_SUPPORTS_NEON)

if(COMPILER_SUPPORTS_AVX2)
    message(STATUS "AVX2 supported")
    target_compile_definitions(flatpack PRIVATE FLATPACK_HAS_AVX2=1)
    target_compile_options(flatpack PRIVATE -mavx2)
elseif(COMPILER_SUPPORTS_SSE2)
    message(STATUS "SSE2 supported")
    target_compile_definitions(flatpack PRIVATE FLATPACK_HAS_SSE2=1)
elseif(COMPILER_SUPPORTS_NEON)
    message(STATUS "NEON supported")
    target_compile_definitions(flatpack PRIVATE FLATPACK_HAS_NEON=1)
else()
    message(STATUS "No SIMD supported, using scalar fallback")
endif()

# Optimizations
target_compile_features(flatpack PRIVATE cxx_std_17)
target_compile_options(flatpack PRIVATE -O3)

# Include directories (for public headers)
target_include_directories(flatpack
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define version macros for users
target_compile_definitions(flatpack
    PUBLIC
    FLATPACK_VERSION_MAJOR=${FLATPACK_VERSION_MAJOR}
    FLATPACK_VERSION_MINOR=${FLATPACK_VERSION_MINOR}
    FLATPACK_VERSION_PATCH=${FLATPACK_VERSION_PATCH}
)

# Add test executable
add_executable(flatpack_test
    tests/test.cpp
)

# Link the flatpack library to the test
target_link_libraries(flatpack_test
    PRIVATE
    flatpack
)

# Enable testing (optional, but recommended)
enable_testing()

# Add test to ctest (optional but useful)
add_test(NAME FlatpackTest COMMAND flatpack_test)
