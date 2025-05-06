// simd.hpp
#pragma once

#if defined(FLATPACK_HAS_AVX2)
#include "simd_avx2.hpp"
#elif defined(FLATPACK_HAS_NEON)
#include "simd_neon.hpp"
#elif defined(FLATPACK_HAS_SSE2)
#include "simd_sse2.hpp"
#else
#include "simd_scalar.hpp"
#endif
