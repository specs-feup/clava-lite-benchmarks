/*
+--------------------------------------------------------------------------+
| CHStone : a suite of benchmark programs for C-based High-Level Synthesis |
| ======================================================================== |
|                                                                          |
| * Collected and Modified : Y. Hara, H. Tomiyama, S. Honda,               |
|                            H. Takada and K. Ishii                        |
|                            Nagoya University, Japan                      |
|                                                                          |
| * Remark :                                                               |
|    1. This source code is modified to unify the formats of the benchmark |
|       programs in CHStone.                                               |
|    2. Test vectors are added for CHStone.                                |
|    3. If "main_result" is 0 at the end of the program, the program is    |
|       correctly executed.                                                |
|    4. Please follow the copyright of each benchmark program.             |
+--------------------------------------------------------------------------+
*/
/*============================================================================

This C source file is part of the SoftFloat IEC/IEEE Floating-point Arithmetic
Package, Release 2b.

Written by John R. Hauser.  This work was made possible in part by the
International Computer Science Institute, located at Suite 600, 1947 Center
Street, Berkeley, California 94704.  Funding was partially provided by the
National Science Foundation under grant MIP-9311980.  The original version
of this code was written as part of a project to build a fixed-point vector
processor in collaboration with the University of California at Berkeley,
overseen by Profs. Nelson Morgan and John Wawrzynek.  More information
is available through the Web page `http://www.cs.berkeley.edu/~jhauser/
arithmetic/SoftFloat.html'.

THIS SOFTWARE IS DISTRIBUTED AS IS, FOR FREE.  Although reasonable effort has
been made to avoid it, THIS SOFTWARE MAY CONTAIN FAULTS THAT WILL AT TIMES
RESULT IN INCORRECT BEHAVIOR.  USE OF THIS SOFTWARE IS RESTRICTED TO PERSONS
AND ORGANIZATIONS WHO CAN AND WILL TAKE FULL RESPONSIBILITY FOR ALL LOSSES,
COSTS, OR OTHER PROBLEMS THEY INCUR DUE TO THE SOFTWARE, AND WHO FURTHERMORE
EFFECTIVELY INDEMNIFY JOHN HAUSER AND THE INTERNATIONAL COMPUTER SCIENCE
INSTITUTE (possibly via similar legal warning) AGAINST ALL LOSSES, COSTS, OR
OTHER PROBLEMS INCURRED BY THEIR CUSTOMERS AND CLIENTS DUE TO THE SOFTWARE.

Derivative works are acceptable, even for commercial purposes, so long as
(1) the source code for the derivative work includes prominent notice that
the work is derivative, and (2) the source code includes prominent notice with
these four paragraphs for those parts of this code that are retained.

=============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "SPARC-GCC.h"
#include "softfloat.h"

/*----------------------------------------------------------------------------
| Floating-point rounding mode, extended double-precision rounding precision,
| and exception flags.
*----------------------------------------------------------------------------*/
int8 float_rounding_mode = float_round_nearest_even;
int8 float_exception_flags = 0;

/*----------------------------------------------------------------------------
| Primitive arithmetic functions, including multi-word arithmetic, and
| division and square root approximations.  (Can be specialized to target if
| desired.)
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
| Shifts `a' right by the number of bits given in `count'.  If any nonzero
| bits are shifted off, they are ``jammed'' into the least significant bit of
| the result by setting the least significant bit to 1.  The value of `count'
| can be arbitrarily large; in particular, if `count' is greater than 64, the
| result will be either 0 or 1, depending on whether `a' is zero or nonzero.
| The result is stored in the location pointed to by `zPtr'.
*----------------------------------------------------------------------------*/

INLINE void
shift64RightJamming(bits64 a, int16 count, bits64 *zPtr)
{
    bits64 z;

    if (count == 0)
    {
        z = a;
    }
    else if (count < 64)
    {
        z = (a >> count) | ((a << ((-count) & 63)) != 0);
    }
    else
    {
        z = (a != 0);
    }
    *zPtr = z;
}

/*----------------------------------------------------------------------------
| Adds the 128-bit value formed by concatenating `a0' and `a1' to the 128-bit
| value formed by concatenating `b0' and `b1'.  Addition is modulo 2^128, so
| any carry out is lost.  The result is broken into two 64-bit pieces which
| are stored at the locations pointed to by `z0Ptr' and `z1Ptr'.
*----------------------------------------------------------------------------*/

INLINE void
add128(bits64 a0, bits64 a1, bits64 b0, bits64 b1, bits64 *z0Ptr,
       bits64 *z1Ptr)
{
    bits64 z1;

    z1 = a1 + b1;
    *z1Ptr = z1;
    *z0Ptr = a0 + b0 + (z1 < a1);
}

/*----------------------------------------------------------------------------
| Subtracts the 128-bit value formed by concatenating `b0' and `b1' from the
| 128-bit value formed by concatenating `a0' and `a1'.  Subtraction is modulo
| 2^128, so any borrow out (carry out) is lost.  The result is broken into two
| 64-bit pieces which are stored at the locations pointed to by `z0Ptr' and
| `z1Ptr'.
*----------------------------------------------------------------------------*/

INLINE void
sub128(bits64 a0, bits64 a1, bits64 b0, bits64 b1, bits64 *z0Ptr,
       bits64 *z1Ptr)
{

    *z1Ptr = a1 - b1;
    *z0Ptr = a0 - b0 - (a1 < b1);
}

/*----------------------------------------------------------------------------
| Multiplies `a' by `b' to obtain a 128-bit product.  The product is broken
| into two 64-bit pieces which are stored at the locations pointed to by
| `z0Ptr' and `z1Ptr'.
*----------------------------------------------------------------------------*/

INLINE void
mul64To128(bits64 a, bits64 b, bits64 *z0Ptr, bits64 *z1Ptr)
{
    bits32 aHigh, aLow, bHigh, bLow;
    bits64 z0, zMiddleA, zMiddleB, z1;

    aLow = a;
    aHigh = a >> 32;
    bLow = b;
    bHigh = b >> 32;
    z1 = ((bits64)aLow) * bLow;
    zMiddleA = ((bits64)aLow) * bHigh;
    zMiddleB = ((bits64)aHigh) * bLow;
    z0 = ((bits64)aHigh) * bHigh;
    zMiddleA += zMiddleB;
    z0 += (((bits64)(zMiddleA < zMiddleB)) << 32) + (zMiddleA >> 32);
    zMiddleA <<= 32;
    z1 += zMiddleA;
    z0 += (z1 < zMiddleA);
    *z1Ptr = z1;
    *z0Ptr = z0;
}

/*----------------------------------------------------------------------------
| Returns an approximation to the 64-bit integer quotient obtained by dividing
| `b' into the 128-bit value formed by concatenating `a0' and `a1'.  The
| divisor `b' must be at least 2^63.  If q is the exact quotient truncated
| toward zero, the approximation returned lies between q and q + 2 inclusive.
| If the exact quotient q is larger than 64 bits, the maximum positive 64-bit
| unsigned integer is returned.
*----------------------------------------------------------------------------*/

static bits64
estimateDiv128To64(bits64 a0, bits64 a1, bits64 b)
{
    bits64 b0, b1;
    bits64 rem0, rem1, term0, term1;
    bits64 z;

    if (b <= a0)
        return LIT64(0xFFFFFFFFFFFFFFFF);
    b0 = b >> 32;
    z = (b0 << 32 <= a0) ? LIT64(0xFFFFFFFF00000000) : (a0 / b0) << 32;
    mul64To128(b, z, &term0, &term1);
    sub128(a0, a1, term0, term1, &rem0, &rem1);
    while (((sbits64)rem0) < 0)
    {
        z -= LIT64(0x100000000);
        b1 = b << 32;
        add128(rem0, rem1, b0, b1, &rem0, &rem1);
    }
    rem0 = (rem0 << 32) | (rem1 >> 32);
    z |= (b0 << 32 <= rem0) ? 0xFFFFFFFF : rem0 / b0;
    return z;
}

/*----------------------------------------------------------------------------
| Returns the number of leading 0 bits before the most-significant 1 bit of
| `a'.  If `a' is zero, 32 is returned.
*----------------------------------------------------------------------------*/

static int8
countLeadingZeros32(bits32 a)
{
    static const int8 countLeadingZerosHigh[256] = {
        8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int8 shiftCount;

    shiftCount = 0;
    if (a < 0x10000)
    {
        shiftCount += 16;
        a <<= 16;
    }
    if (a < 0x1000000)
    {
        shiftCount += 8;
        a <<= 8;
    }
    shiftCount += countLeadingZerosHigh[a >> 24];
    return shiftCount;
}

/*----------------------------------------------------------------------------
| Returns the number of leading 0 bits before the most-significant 1 bit of
| `a'.  If `a' is zero, 64 is returned.
*----------------------------------------------------------------------------*/

static int8
countLeadingZeros64(bits64 a)
{
    int8 shiftCount;

    shiftCount = 0;
    if (a < ((bits64)1) << 32)
    {
        shiftCount += 32;
    }
    else
    {
        a >>= 32;
    }
    shiftCount += countLeadingZeros32(a);
    return shiftCount;
}

/*----------------------------------------------------------------------------
| Functions and definitions to determine:  (1) whether tininess for underflow
| is detected before or after rounding by default, (2) what (if anything)
| happens when exceptions are raised, (3) how signaling NaNs are distinguished
| from quiet NaNs, (4) the default generated quiet NaNs, and (5) how NaNs
| are propagated from function inputs to output.  These details are target-
| specific.
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
| Underflow tininess-detection mode, statically initialized to default value.
| (The declaration in `softfloat.h' must match the `int8' type here.)
*----------------------------------------------------------------------------*/
#define float_detect_tininess float_tininess_before_rounding

/*----------------------------------------------------------------------------
| Raises the exceptions specified by `flags'.  Floating-point traps can be
| defined here if desired.  It is currently not possible for such a trap
| to substitute a result value.  If traps are not implemented, this routine
| should be simply `float_exception_flags |= flags;'.
*----------------------------------------------------------------------------*/

void float_raise(int8 flags)
{
    float_exception_flags |= flags;
}

/*----------------------------------------------------------------------------
| The pattern for a default generated double-precision NaN.
*----------------------------------------------------------------------------*/
#define float64_default_nan LIT64(0x7FFFFFFFFFFFFFFF)

/*----------------------------------------------------------------------------
| Returns 1 if the double-precision floating-point value `a' is a NaN;
| otherwise returns 0.
*----------------------------------------------------------------------------*/

flag float64_is_nan(float64 a)
{

    return (LIT64(0xFFE0000000000000) < (bits64)(a << 1));
}

/*----------------------------------------------------------------------------
| Returns 1 if the double-precision floating-point value `a' is a signaling
| NaN; otherwise returns 0.
*----------------------------------------------------------------------------*/

flag float64_is_signaling_nan(float64 a)
{

    return (((a >> 51) & 0xFFF) == 0xFFE) && (a & LIT64(0x0007FFFFFFFFFFFF));
}

/*----------------------------------------------------------------------------
| Takes two double-precision floating-point values `a' and `b', one of which
| is a NaN, and returns the appropriate NaN result.  If either `a' or `b' is a
| signaling NaN, the invalid exception is raised.
*----------------------------------------------------------------------------*/

static float64
propagateFloat64NaN(float64 a, float64 b)
{
    flag aIsNaN, aIsSignalingNaN, bIsNaN, bIsSignalingNaN;

    aIsNaN = float64_is_nan(a);
    aIsSignalingNaN = float64_is_signaling_nan(a);
    bIsNaN = float64_is_nan(b);
    bIsSignalingNaN = float64_is_signaling_nan(b);
    a |= LIT64(0x0008000000000000);
    b |= LIT64(0x0008000000000000);
    if (aIsSignalingNaN | bIsSignalingNaN)
        float_raise(float_flag_invalid);
    return bIsSignalingNaN ? b : aIsSignalingNaN ? a
                             : bIsNaN            ? b
                                                 : a;
}

/*----------------------------------------------------------------------------
| Returns the fraction bits of the double-precision floating-point value `a'.
*----------------------------------------------------------------------------*/
bits64 extractFloat64Frac(float64 a) __attribute__((always_inline));
bits64 extractFloat64Frac(float64 a)
{
    return a & LIT64(0x000FFFFFFFFFFFFF);
}

/*----------------------------------------------------------------------------
| Returns the exponent bits of the double-precision floating-point value `a'.
*----------------------------------------------------------------------------*/
int16 extractFloat64Exp(float64 a) __attribute__((always_inline));
int16 extractFloat64Exp(float64 a)
{
    return (a >> 52) & 0x7FF;
}

/*----------------------------------------------------------------------------
| Returns the sign bit of the double-precision floating-point value `a'.
*----------------------------------------------------------------------------*/
flag extractFloat64Sign(float64 a) __attribute__((always_inline));
flag extractFloat64Sign(float64 a)
{

    return a >> 63;
}
/*----------------------------------------------------------------------------
| Normalizes the subnormal double-precision floating-point value represented
| by the denormalized significand `aSig'.  The normalized exponent and
| significand are stored at the locations pointed to by `zExpPtr' and
| `zSigPtr', respectively.
*----------------------------------------------------------------------------*/
void normalizeFloat64Subnormal(bits64 aSig, int16 *zExpPtr, bits64 *zSigPtr) __attribute__((always_inline));
void normalizeFloat64Subnormal(bits64 aSig, int16 *zExpPtr, bits64 *zSigPtr)
{
    int8 shiftCount;

    shiftCount = countLeadingZeros64(aSig) - 11;
    *zSigPtr = aSig << shiftCount;
    *zExpPtr = 1 - shiftCount;
}

/*----------------------------------------------------------------------------
| Packs the sign `zSign', exponent `zExp', and significand `zSig' into a
| double-precision floating-point value, returning the result.  After being
| shifted into the proper positions, the three fields are simply added
| together to form the result.  This means that any integer portion of `zSig'
| will be added into the exponent.  Since a properly normalized significand
| will have an integer portion equal to 1, the `zExp' input should be 1 less
| than the desired result exponent whenever `zSig' is a complete, normalized
| significand.
*----------------------------------------------------------------------------*/
float64 packFloat64(flag zSign, int16 zExp, bits64 zSig) __attribute__((always_inline));
float64 packFloat64(flag zSign, int16 zExp, bits64 zSig)
{
    return (((bits64)zSign) << 63) + (((bits64)zExp) << 52) + zSig;
}

/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand `zSig', and returns the proper double-precision floating-
| point value corresponding to the abstract input.  Ordinarily, the abstract
| value is simply rounded and packed into the double-precision format, with
| the inexact exception raised if the abstract input cannot be represented
| exactly.  However, if the abstract value is too large, the overflow and
| inexact exceptions are raised and an infinity or maximal finite value is
| returned.  If the abstract value is too small, the input value is rounded
| to a subnormal number, and the underflow and inexact exceptions are raised
| if the abstract input cannot be represented exactly as a subnormal double-
| precision floating-point number.
|     The input significand `zSig' has its binary point between bits 62
| and 61, which is 10 bits to the left of the usual location.  This shifted
| significand must be normalized or smaller.  If `zSig' is not normalized,
| `zExp' must be 0; in that case, the result returned is a subnormal number,
| and it must not require rounding.  In the usual case that `zSig' is
| normalized, `zExp' must be 1 less than the ``true'' floating-point exponent.
| The handling of underflow and overflow follows the IEC/IEEE Standard for
| Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
float64 roundAndPackFloat64(flag zSign, int16 zExp, bits64 zSig) __attribute__((always_inline));
float64 roundAndPackFloat64(flag zSign, int16 zExp, bits64 zSig)
{
    int8 roundingMode;
    flag roundNearestEven, isTiny;
    int16 roundIncrement, roundBits;

    roundingMode = float_rounding_mode;
    roundNearestEven = (roundingMode == float_round_nearest_even);
    roundIncrement = 0x200;
    if (!roundNearestEven)
    {
        if (roundingMode == float_round_to_zero)
        {
            roundIncrement = 0;
        }
        else
        {
            roundIncrement = 0x3FF;
            if (zSign)
            {
                if (roundingMode == float_round_up)
                    roundIncrement = 0;
            }
            else
            {
                if (roundingMode == float_round_down)
                    roundIncrement = 0;
            }
        }
    }
    roundBits = zSig & 0x3FF;
    if (0x7FD <= (bits16)zExp)
    {
        if ((0x7FD < zExp) || ((zExp == 0x7FD) && ((sbits64)(zSig + roundIncrement) < 0)))
        {
            float_raise(float_flag_overflow | float_flag_inexact);
            return packFloat64(zSign, 0x7FF, 0) - (roundIncrement == 0);
        }
        if (zExp < 0)
        {
            isTiny = (float_detect_tininess == float_tininess_before_rounding) || (zExp < -1) || (zSig + roundIncrement < LIT64(0x8000000000000000));
            shift64RightJamming(zSig, -zExp, &zSig);
            zExp = 0;
            roundBits = zSig & 0x3FF;
            if (isTiny && roundBits)
                float_raise(float_flag_underflow);
        }
    }
    if (roundBits)
        float_exception_flags |= float_flag_inexact;
    zSig = (zSig + roundIncrement) >> 10;
    zSig &= ~(((roundBits ^ 0x200) == 0) & roundNearestEven);
    if (zSig == 0)
        zExp = 0;
    return packFloat64(zSign, zExp, zSig);
}

/*----------------------------------------------------------------------------
| Returns the result of dividing the double-precision floating-point value `a'
| by the corresponding value `b'.  The operation is performed according to
| the IEC/IEEE Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
float64 float64_div(float64 a, float64 b)
{
    flag aSign, bSign, zSign;
    int16 aExp, bExp, zExp;
    bits64 aSig, bSig, zSig;
    bits64 rem0, rem1, term0, term1;

    aSig = extractFloat64Frac(a);
    aExp = extractFloat64Exp(a);
    aSign = extractFloat64Sign(a);
    bSig = extractFloat64Frac(b);
    bExp = extractFloat64Exp(b);
    bSign = extractFloat64Sign(b);
    zSign = aSign ^ bSign;
    if (aExp == 0x7FF)
    {
        if (aSig)
            return propagateFloat64NaN(a, b);
        if (bExp == 0x7FF)
        {
            if (bSig)
                return propagateFloat64NaN(a, b);
            float_raise(float_flag_invalid);
            return float64_default_nan;
        }
        return packFloat64(zSign, 0x7FF, 0);
    }
    if (bExp == 0x7FF)
    {
        if (bSig)
            return propagateFloat64NaN(a, b);
        return packFloat64(zSign, 0, 0);
    }
    if (bExp == 0)
    {
        if (bSig == 0)
        {
            if ((aExp | aSig) == 0)
            {
                float_raise(float_flag_invalid);
                return float64_default_nan;
            }
            float_raise(float_flag_divbyzero);
            return packFloat64(zSign, 0x7FF, 0);
        }
        normalizeFloat64Subnormal(bSig, &bExp, &bSig);
    }
    if (aExp == 0)
    {
        if (aSig == 0)
            return packFloat64(zSign, 0, 0);
        normalizeFloat64Subnormal(aSig, &aExp, &aSig);
    }
    zExp = aExp - bExp + 0x3FD;
    aSig = (aSig | LIT64(0x0010000000000000)) << 10;
    bSig = (bSig | LIT64(0x0010000000000000)) << 11;
    if (bSig <= (aSig + aSig))
    {
        aSig >>= 1;
        ++zExp;
    }
    zSig = estimateDiv128To64(aSig, 0, bSig);
    if ((zSig & 0x1FF) <= 2)
    {
        mul64To128(bSig, zSig, &term0, &term1);
        sub128(aSig, 0, term0, term1, &rem0, &rem1);
        while ((sbits64)rem0 < 0)
        {
            --zSig;
            add128(rem0, rem1, 0, bSig, &rem0, &rem1);
        }
        zSig |= (rem1 != 0);
    }
    return roundAndPackFloat64(zSign, zExp, zSig);
}
/*
+--------------------------------------------------------------------------+
| * Test Vectors (added for CHStone)                                       |
|     a_input, b_input : input data                                        |
|     z_output : expected output data                                      |
+--------------------------------------------------------------------------+
*/
#define N 22

const float64 a_input[N] = {
    0x7FFF000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x7FF0000000000000ULL, /* inf */
    0x7FF0000000000000ULL, /* inf */
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x0000000000000000ULL, /* 0.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x0000000000000000ULL, /* 0.0 */
    0x8000000000000000ULL, /* -0.0 */
    0x4008000000000000ULL, /* 3.0 */
    0xC008000000000000ULL, /* -3.0 */
    0x4008000000000000ULL, /* 3.0 */
    0xC008000000000000ULL, /* -3.0 */
    0x4000000000000000ULL, /* 2.0 */
    0xC000000000000000ULL, /* -2.0 */
    0x4000000000000000ULL, /* 2.0 */
    0xC000000000000000ULL, /* -2.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0xBFF0000000000000ULL, /* -1.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0xBFF0000000000000ULL  /* -1.0 */
};

const float64 b_input[N] = {
    0x3FF0000000000000ULL, /* 1.0 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x3FF0000000000000ULL, /* 1.0 */
    0x7FF8000000000000ULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x0000000000000000ULL, /* 0.0 */
    0x0000000000000000ULL, /* 0.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x3FF0000000000000ULL, /* 1.0 */
    0x4000000000000000ULL, /* 2.0 */
    0x4000000000000000ULL, /* 2.0 */
    0xC000000000000000ULL, /* 2.0 */
    0xC000000000000000ULL, /* -2.0 */
    0x4010000000000000ULL, /* 4.0 */
    0x4010000000000000ULL, /* 4.0 */
    0xC010000000000000ULL, /* -4.0 */
    0xC010000000000000ULL, /* -4.0 */
    0x3FF8000000000000ULL, /* 1.5 */
    0x3FF8000000000000ULL, /* 1.5 */
    0xBFF8000000000000ULL, /* -1.5 */
    0xBFF8000000000000ULL  /* -1.5 */
};

const float64 z_output[N] = {
    0x7FFF000000000000ULL, /* nan */
    0x7FF8000000000000ULL, /* nan */
    0x7FFFFFFFFFFFFFFFULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x7FF8000000000000ULL, /* nan */
    0x0000000000000000ULL, /* 0.0 */
    0x7FFFFFFFFFFFFFFFULL, /* nan */
    0x7FF0000000000000ULL, /* inf */
    0x0000000000000000ULL, /* 0.0 */
    0x8000000000000000ULL, /* -0.0 */
    0x3FF8000000000000ULL, /* 1.5 */
    0xBFF8000000000000ULL, /* -1.5 */
    0xBFF8000000000000ULL, /* 1.5 */
    0x3FF8000000000000ULL, /* -1.5 */
    0x3FE0000000000000ULL, /* 0.5 */
    0xBFE0000000000000ULL, /* 5.0 */
    0xBFE0000000000000ULL, /* -5.0 */
    0x3FE0000000000000ULL, /* 0.5 */
    0x3FE5555555555555ULL, /* 0.666667 */
    0xBFE5555555555555ULL, /* -0.666667 */
    0xBFE5555555555555ULL, /* -0.666667 */
    0x3FE5555555555555ULL  /* 0.666667 */
};

int main(int argc, char **argv)
{
    int main_result;
    int i;
    float64 x1, x2;
    main_result = 0;
	#pragma kernel
    for (i = 0; i < N; i++)
    {
        float64 result;
        x1 = a_input[i];
        x2 = b_input[i];
        result = float64_div(x1, x2);
        main_result += (result != z_output[i]);

        //printf("a_input=%016llx b_input=%016llx expected=%016llx output=%016llx\n",
        //a_input[i], b_input[i], z_output[i], result);
    }
    //printf("%d\n", main_result);
    return main_result;
}
