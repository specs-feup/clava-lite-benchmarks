/*
+--------------------------------------------------------------------------+
| * Test Vectors (added for CHStone)                                       |
|     a_input, b_input : input data                                        |
|     z_output : expected output data                                      |
+--------------------------------------------------------------------------+
*/
#include "softfloat.h"

#define N 46
const float64 a_input[N] = {
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x4000000000000000ULL, /* 2.0 */
    0x4000000000000000ULL, /* 2.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x0000000000000000ULL, /* 0.0 */
    0x3FF8000000000000ULL, /* 1.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x0000000000000000ULL, /* 0.0 */
    0x3FF8000000000000ULL, /* 1.5 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0xC000000000000000ULL, /* -2.0 */
    0xC000000000000000ULL, /* -2.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0x8000000000000000ULL, /* -0.0 */
    0xBFF8000000000000ULL, /* -1.5 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0x8000000000000000ULL, /* -0.0 */
    0xBFF8000000000000ULL, /* -1.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x0000000000000000ULL, /* 0.0 */
    0x3FF8000000000000ULL, /* 1.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x3FF0000000000000ULL, /* 1.0 */
    0x4000000000000000ULL, /* 2.0 */
    0xFFF0000000000000ULL, /* -inf */
    0xFFF0000000000000ULL, /* -inf */
    0xBFF0000000000000ULL, /* -1.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0x8000000000000000ULL, /* -0.0 */
    0xBFF8000000000000ULL, /* -1.5 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0xBFF0000000000000ULL, /* -1.0 */
    0xC000000000000000ULL  /* -2.0 */
};

const float64 b_input[N] = {
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x0000000000000000ULL, /* 0.0 */
    0x3FF8000000000000ULL, /* 1.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x4000000000000000ULL, /* 2.0 */
    0x4000000000000000ULL, /* 2.0 */
    0x7FF0000000000000ULL, /* inf */
    0x7FF0000000000000ULL, /* inf */
    0x0000000000000000ULL, /* 0.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0x8000000000000000ULL, /* -0.0 */
    0xBFF8000000000000ULL, /* -1.5 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0xC000000000000000ULL, /* -2.0 */
    0xC000000000000000ULL, /* -2.0 */
    0xFFF0000000000000ULL, /* -inf */
    0xFFF0000000000000ULL, /* -inf */
    0x8000000000000000ULL, /* -inf */
    0xBFF0000000000000ULL, /* -1.0 */
    0xFFF0000000000000ULL, /* -inf */
    0xFFF0000000000000ULL, /* -inf */
    0xBFF0000000000000ULL, /* -1.0 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0xBFF0000000000000ULL, /* -1.0 */
    0xC000000000000000ULL, /* -2.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0x8000000000000000ULL, /* -0.0 */
    0xBFF8000000000000ULL, /* -1.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x3FF0000000000000ULL, /* 1.0 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x3FF0000000000000ULL, /* 1.0 */
    0x4000000000000000ULL, /* 2.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x0000000000000000ULL, /* 0.0 */
    0x3FF8000000000000ULL  /* 1.5 */
};

const float64 z_output[N] = {
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x4000000000000000ULL, /* 2.0 */
    0x400C000000000000ULL, /* 3.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x4000000000000000ULL, /* 2.0 */
    0x400C000000000000ULL, /* 3.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x0000000000000000ULL, /* 0.0 */
    0x4004000000000000ULL, /* 2.5 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0xC000000000000000ULL, /* -2.0 */
    0xC00C000000000000ULL, /* -3.5 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0xC000000000000000ULL, /* -2.0 */
    0xC00C000000000000ULL, /* -3.5 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0x8000000000000000ULL, /* -0.0 */
    0xC004000000000000ULL, /* -2.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FFFFFFFFFFFFFFFULL, /* nan */
    0x0000000000000000ULL, /* 0.0 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0xBFF0000000000000ULL, /* -1.0 */
    0xBFE0000000000000ULL, /* -0.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FE0000000000000ULL, /* 0.5 */
    0x7FF8000000000000ULL, /* nan */
    0x7FFFFFFFFFFFFFFFULL, /* nan */
    0x0000000000000000ULL, /* 0.0 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FE0000000000000ULL, /* 0.5 */
    0xFFF8000000000000ULL, /* nan */
    0xFFF0000000000000ULL, /* -inf */
    0xBFF0000000000000ULL, /* -1.0 */
    0xBFE0000000000000ULL  /* -0.5 */
};