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
/*
 * Copyright (C) 2008
 * Y. Hara, H. Tomiyama, S. Honda, H. Takada and K. Ishii
 * Nagoya University, Japan
 * All rights reserved.
 *
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis. The authors disclaims any and all warranties,
 * whether express, implied, or statuary, including any implied warranties or
 * merchantability or of fitness for a particular purpose. In no event shall the
 * copyright-holder be liable for any incidental, punitive, or consequential damages
 * of any kind whatsoever arising from the use of these programs. This disclaimer
 * of warranty extends to the user of these programs and user's customers, employees,
 * agents, transferees, successors, and assigns.
 *
 */
#include <stdio.h>

#include "SPARC-GCC.h"
#include "softfloat.h"
#include "input.h"

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

INLINE void shift64RightJamming(bits64 a, int16 count, bits64 *zPtr)
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
| Returns the number of leading 0 bits before the most-significant 1 bit of
| `a'.  If `a' is zero, 32 is returned.
*----------------------------------------------------------------------------*/

static int8 countLeadingZeros32(bits32 a)
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

static int8 countLeadingZeros64(bits64 a)
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
| Underflow tininess-detection mode, statically initialized to default value.
| (The declaration in `softfloat.h' must match the `int8' type here.)
*----------------------------------------------------------------------------*/
#define float_detect_tininess float_tininess_before_rounding

/*----------------------------------------------------------------------------
| Functions and definitions to determine:  (1) whether tininess for underflow
| is detected before or after rounding by default, (2) what (if anything)
| happens when exceptions are raised, (3) how signaling NaNs are distinguished
| from quiet NaNs, (4) the default generated quiet NaNs, and (5) how NaNs
| are propagated from function inputs to output.  These details are target-
| specific.
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

static float64 propagateFloat64NaN(float64 a, float64 b)
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

INLINE bits64 extractFloat64Frac(float64 a)
{
    return a & LIT64(0x000FFFFFFFFFFFFF);
}

/*----------------------------------------------------------------------------
| Returns the exponent bits of the double-precision floating-point value `a'.
*----------------------------------------------------------------------------*/

INLINE int16 extractFloat64Exp(float64 a)
{
    return (a >> 52) & 0x7FF;
}

/*----------------------------------------------------------------------------
| Returns the sign bit of the double-precision floating-point value `a'.
*----------------------------------------------------------------------------*/

INLINE flag extractFloat64Sign(float64 a)
{
    return a >> 63;
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

INLINE float64 packFloat64(flag zSign, int16 zExp, bits64 zSig)
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
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand `zSig', and returns the proper double-precision floating-
| point value corresponding to the abstract input.  This routine is just like
| `roundAndPackFloat64' except that `zSig' does not have to be normalized.
| Bit 63 of `zSig' must be zero, and `zExp' must be 1 less than the ``true''
| floating-point exponent.
*----------------------------------------------------------------------------*/

static float64 normalizeRoundAndPackFloat64(flag zSign, int16 zExp, bits64 zSig)
{
    int8 shiftCount;

    shiftCount = countLeadingZeros64(zSig) - 1;
    return roundAndPackFloat64(zSign, zExp - shiftCount, zSig << shiftCount);
}

/*----------------------------------------------------------------------------
| Returns the result of adding the absolute values of the double-precision
| floating-point values `a' and `b'.  If `zSign' is 1, the sum is negated
| before being returned.  `zSign' is ignored if the result is a NaN.
| The addition is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

float64 addFloat64Sigs(float64 a, float64 b, flag zSign)
{
    int16 aExp, bExp, zExp;
    bits64 aSig, bSig, zSig;
    int16 expDiff;

    aSig = extractFloat64Frac(a);
    aExp = extractFloat64Exp(a);
    bSig = extractFloat64Frac(b);
    bExp = extractFloat64Exp(b);
    expDiff = aExp - bExp;
    aSig <<= 9;
    bSig <<= 9;
    if (0 < expDiff)
    {
        if (aExp == 0x7FF)
        {
            if (aSig)
                return propagateFloat64NaN(a, b);
            return a;
        }
        if (bExp == 0)
            --expDiff;
        else
            bSig |= LIT64(0x2000000000000000);
        shift64RightJamming(bSig, expDiff, &bSig);
        zExp = aExp;
    }
    else if (expDiff < 0)
    {
        if (bExp == 0x7FF)
        {
            if (bSig)
                return propagateFloat64NaN(a, b);
            return packFloat64(zSign, 0x7FF, 0);
        }
        if (aExp == 0)
            ++expDiff;
        else
        {
            aSig |= LIT64(0x2000000000000000);
        }
        shift64RightJamming(aSig, -expDiff, &aSig);
        zExp = bExp;
    }
    else
    {
        if (aExp == 0x7FF)
        {
            if (aSig | bSig)
                return propagateFloat64NaN(a, b);
            return a;
        }
        if (aExp == 0)
            return packFloat64(zSign, 0, (aSig + bSig) >> 9);
        zSig = LIT64(0x4000000000000000) + aSig + bSig;
        zExp = aExp;
        return roundAndPackFloat64(zSign, zExp, zSig);
    }
    aSig |= LIT64(0x2000000000000000);
    zSig = (aSig + bSig) << 1;
    --zExp;
    if ((sbits64)zSig < 0)
    {
        zSig = aSig + bSig;
        ++zExp;
    }
    return roundAndPackFloat64(zSign, zExp, zSig);
}

/*----------------------------------------------------------------------------
| Returns the result of subtracting the absolute values of the double-
| precision floating-point values `a' and `b'.  If `zSign' is 1, the
| difference is negated before being returned.  `zSign' is ignored if the
| result is a NaN.  The subtraction is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

float64 subFloat64Sigs(float64 a, float64 b, flag zSign)
{
    int16 aExp, bExp, zExp;
    bits64 aSig, bSig, zSig;
    int16 expDiff;

    aSig = extractFloat64Frac(a);
    aExp = extractFloat64Exp(a);
    bSig = extractFloat64Frac(b);
    bExp = extractFloat64Exp(b);
    expDiff = aExp - bExp;
    aSig <<= 10;
    bSig <<= 10;
    if (0 < expDiff)
    {
        if (aExp == 0x7FF)
        {
            if (aSig)
                return propagateFloat64NaN(a, b);
            return a;
        }
        if (bExp == 0)
            --expDiff;
        else
            bSig |= LIT64(0x4000000000000000);
        shift64RightJamming(bSig, expDiff, &bSig);
        aSig |= LIT64(0x4000000000000000);

        zSig = aSig - bSig;
        zExp = aExp;

        --zExp;
        return normalizeRoundAndPackFloat64(zSign, zExp, zSig);
    }
    if (expDiff < 0)
    {
        if (bExp == 0x7FF)
        {
            if (bSig)
                return propagateFloat64NaN(a, b);
            return packFloat64(zSign ^ 1, 0x7FF, 0);
        }
        if (aExp == 0)
            ++expDiff;
        else
            aSig |= LIT64(0x4000000000000000);
        shift64RightJamming(aSig, -expDiff, &aSig);
        bSig |= LIT64(0x4000000000000000);

        if (aExp == 0x7FF)
        {
            if (aSig)
                return propagateFloat64NaN(a, b);
            return a;
        }
        if (bExp == 0)
            --expDiff;
        else
            bSig |= LIT64(0x4000000000000000);
        shift64RightJamming(bSig, expDiff, &bSig);
        aSig |= LIT64(0x4000000000000000);
        zSig = aSig - bSig;
        zExp = aExp;
        --zExp;
        return normalizeRoundAndPackFloat64(zSign, zExp, zSig);
    }
    if (aExp == 0x7FF)
    {
        if (aSig | bSig)
            return propagateFloat64NaN(a, b);
        float_raise(float_flag_invalid);
        return float64_default_nan;
    }
    if (aExp == 0)
    {
        aExp = 1;
        bExp = 1;
    }
    if (bSig < aSig)
    {
        zSig = aSig - bSig;
        zExp = aExp;
        --zExp;
        return normalizeRoundAndPackFloat64(zSign, zExp, zSig);
    }
    if (aSig < bSig)
    {
        zSig = bSig - aSig;
        zExp = bExp;
        zSign ^= 1;
        --zExp;
        return normalizeRoundAndPackFloat64(zSign, zExp, zSig);
    }
    return packFloat64(float_rounding_mode == float_round_down, 0, 0);
}

/*----------------------------------------------------------------------------
| Returns the result of adding the double-precision floating-point values `a'
| and `b'.  The operation is performed according to the IEC/IEEE Standard for
| Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

float64 float64_add(float64 a, float64 b)
{
    flag aSign, bSign;

    aSign = extractFloat64Sign(a);
    bSign = extractFloat64Sign(b);
    if (aSign == bSign)
        return addFloat64Sigs(a, b, aSign);
    else
        return subFloat64Sigs(a, b, aSign);
}

int main()
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
        result = float64_add(x1, x2);
        main_result += (result != z_output[i]);

        // printf("a_input=%016llx b_input=%016llx expected=%016llx output=%016llx\n",
        //        a_input[i], b_input[i], z_output[i], result);
    }
    // printf("%d\n", main_result);
    return main_result;
}
