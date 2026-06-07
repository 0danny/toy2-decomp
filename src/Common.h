#pragma once

// Typedefs
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef __int64 int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

typedef float float32_t;
typedef double float64_t;

// The MSVC 6 way of declaring static asserts
#define STATIC_ASSERT_GLUE(a, b) a##b
#define STATIC_ASSERT_JOIN(a, b) STATIC_ASSERT_GLUE(a, b)

#define STATIC_ASSERT(expr) typedef char STATIC_ASSERT_JOIN(static_assert_failed_at_line_, __LINE__)[(expr) ? 1 : -1]
