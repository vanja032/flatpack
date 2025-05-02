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
