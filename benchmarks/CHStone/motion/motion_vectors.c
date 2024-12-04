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
#include "input.h"

#define Num 2048

#define _ANSI_ARGS_(x) x

#define ERROR (-1)

#define SEQUENCE_END_CODE 0x1B7

/* mv_format */
#define MV_FIELD 0

const int inPMV[2][2][2] = {{{45, 207}, {70, 41}}, {{4, 180}, {120, 216}}};
const int inmvfs[2][2] = {{232, 200}, {32, 240}};
const int outPMV[2][2][2] =
    {{{1566, 206}, {70, 41}}, {{1566, 206}, {120, 216}}};
const int outmvfs[2][2] = {{0, 200}, {0, 240}};

int evalue;

/* NOTE: the VLC tables are in a flash format---a transformation
   of the tables in Annex B to a form more convenient towards
   parallel (more than one-bit-at-a-time) decoding */

/* Table B-10, motion_code, codes 0001 ... 01xx */
const char MVtab0[8][2] = {
    {ERROR, 0}, {3, 3}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}};

/* Table B-10, motion_code, codes 0000011 ... 000011x */
const char MVtab1[8][2] = {
    {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {7, 6}, {6, 6}, {5, 6}, {4, 5}, {4, 5}};

/* Table B-10, motion_code, codes 0000001100 ... 000001011x */
const char MVtab2[12][2] = {
    {16, 9}, {15, 9}, {14, 9}, {13, 9}, {12, 9}, {11, 9}, {10, 8}, {10, 8}, {9, 8}, {9, 8}, {8, 8}, {8, 8}};

/* choose between declaration (GLOBAL undefined)
 * and definition (GLOBAL defined)
 * GLOBAL is defined in exactly one file mpeg2dec.c)
 */

/* Get_Bits.c */

void Fill_Buffer _ANSI_ARGS_((void));
unsigned int Show_Bits _ANSI_ARGS_((int n));
unsigned int Get_Bits1 _ANSI_ARGS_((void));
void Flush_Buffer _ANSI_ARGS_((int n));
unsigned int Get_Bits _ANSI_ARGS_((int n));
int Get_Byte _ANSI_ARGS_((void));

/* getvlc.c */
int Get_motion_code _ANSI_ARGS_((void));
int Get_dmvector _ANSI_ARGS_((void));
int Get_coded_block_pattern _ANSI_ARGS_((void));

/* motion.c */
void motion_vector
    _ANSI_ARGS_((int *PMV, int *dmvector, int h_r_size, int v_r_size, int dmv,
                 int mvscale, int full_pel_vector));

int System_Stream_Flag;

unsigned char ld_Rdbfr[2048];
unsigned char *ld_Rdptr, *ld_Rdmax;
unsigned int ld_Bfr;
int ld_Incnt;

void Initialize_Buffer()
{
    ld_Incnt = 0;
    ld_Rdptr = ld_Rdbfr + 2048;
    ld_Rdmax = ld_Rdptr;
    ld_Bfr = 68157440;
    Flush_Buffer(0); /* fills valid data into bfr */
}

/* initialize buffer, call once before first getbits or showbits */
int read(unsigned char *s1, const unsigned char *s2, int n)
{
    unsigned char *p1;
    const unsigned char *p2;
    int n_tmp;

    p1 = s1;
    p2 = s2;
    n_tmp = n;

    while (n_tmp-- > 0)
    {
        *p1 = *p2;

        p1++;

        p2++;
    }

    return n;
}

void Fill_Buffer()
{
    int Buffer_Level;
    unsigned char *p;
    p = ld_Rdbfr;

    Buffer_Level = read(ld_Rdbfr, inRdbfr, 2048);
    ld_Rdptr = ld_Rdbfr;

    if (System_Stream_Flag)
        ld_Rdmax -= 2048;

    /* end of the bitstream file */
    if (Buffer_Level < 2048)
    {
        /* just to be safe */
        if (Buffer_Level < 0)
            Buffer_Level = 0;

        /* pad until the next to the next 32-bit word boundary */
        while (Buffer_Level & 3)
            ld_Rdbfr[Buffer_Level++] = 0;

        /* pad the buffer with sequence end codes */
        while (Buffer_Level < 2048)
        {
            ld_Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE >> 24;
            ld_Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE >> 16;
            ld_Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE >> 8;
            ld_Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE & 0xff;
        }
    }
}

unsigned int
Show_Bits(int N)
{
    return ld_Bfr >> (unsigned)(32 - N) % 32;
}

/* return next bit (could be made faster than Get_Bits(1)) */

unsigned int
Get_Bits1()
{
    return Get_Bits(1);
}

/* advance by n bits */
void Flush_Buffer(int N) __attribute__((always_inline));
void Flush_Buffer(int N)
{
    int Incnt;

    ld_Bfr <<= N;

    Incnt = ld_Incnt -= N;

    if (Incnt <= 24)
    {
        if (ld_Rdptr < ld_Rdbfr + 2044)
        {
            do
            {
                ld_Bfr |= *ld_Rdptr++ << (24 - Incnt);
                Incnt += 8;
            } while (Incnt <= 24);
        }
        else
        {
            do
            {
                if (ld_Rdptr >= ld_Rdbfr + 2048)
                    Fill_Buffer();
                ld_Bfr |= *ld_Rdptr++ << (24 - Incnt);
                Incnt += 8;
            } while (Incnt <= 24);
        }
        ld_Incnt = Incnt;
    }
}

/* return next n bits (right adjusted) */

unsigned int
Get_Bits(int N)
{
    unsigned int Val;

    Val = Show_Bits(N);
    Flush_Buffer(N);

    return Val;
}

int Get_motion_code() __attribute__((always_inline));
int Get_motion_code()
{
    int code;

    if (Get_Bits1())
    {
        return 0;
    }

    code = Show_Bits(9);
    if (code >= 64)
    {
        code >>= 6;
        Flush_Buffer(MVtab0[code][1]);

        return Get_Bits1() ? -MVtab0[code][0] : MVtab0[code][0];
    }

    if (code >= 24)
    {
        code >>= 3;
        Flush_Buffer(MVtab1[code][1]);

        return Get_Bits1() ? -MVtab1[code][0] : MVtab1[code][0];
    }

    if ((code -= 12) < 0)
        return 0;

    Flush_Buffer(MVtab2[code][1]);
    return Get_Bits1() ? -MVtab2[code][0] : MVtab2[code][0];
}

/* get differential motion vector (for dual prime prediction) */
int Get_dmvector()
{

    if (Get_Bits(1))
    {
        return Get_Bits(1) ? -1 : 1;
    }
    else
    {
        return 0;
    }
}

/* private prototypes */
static void decode_motion_vector
    _ANSI_ARGS_((int *pred, int r_size, int motion_code,
                 int motion_residualesidual, int full_pel_vector));

/* ISO/IEC 13818-2 sections 6.2.5.2, 6.3.17.2, and 7.6.3: Motion vectors */
void motion_vectors(int PMV[2][2][2], int dmvector[2],
                    int motion_vertical_field_select[2][2], int s,
                    int motion_vector_count, int mv_format,
                    int h_r_size, int v_r_size, int dmv,
                    int mvscale)
{
    if (motion_vector_count == 1)
    {
        if (mv_format == MV_FIELD && !dmv)
        {
            motion_vertical_field_select[1][s] =
                motion_vertical_field_select[0][s] = Get_Bits(1);
        }

        motion_vector(PMV[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale,
                      0);

        /* update other motion vector predictors */
        PMV[1][s][0] = PMV[0][s][0];
        PMV[1][s][1] = PMV[0][s][1];
    }
    else
    {
        motion_vertical_field_select[0][s] = Get_Bits(1);

        motion_vector(PMV[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale,
                      0);

        motion_vertical_field_select[1][s] = Get_Bits(1);

        motion_vector(PMV[1][s], dmvector, h_r_size, v_r_size, dmv, mvscale,
                      0);
    }
}

/* get and decode motion vector and differential motion vector
   for one prediction */
void motion_vector(int *PMV, int *dmvector, int h_r_size, int v_r_size,
                   int dmv /* MPEG-2 only: field vector in frame pic */,
                   int mvscale /* MPEG-2 only: get differential motion vectors */,
                   int full_pel_vector /* MPEG-1 only */)
{
    int motion_code;
    int motion_residual;

    /* horizontal component */
    /* ISO/IEC 13818-2 Table B-10 */
    motion_code = Get_motion_code();

    motion_residual = (h_r_size != 0 && motion_code != 0) ? Get_Bits(h_r_size) : 0;

    decode_motion_vector(&PMV[0], h_r_size, motion_code, motion_residual,
                         full_pel_vector);

    if (dmv)
        dmvector[0] = Get_dmvector();

    /* vertical component */
    motion_code = Get_motion_code();
    motion_residual = (v_r_size != 0 && motion_code != 0) ? Get_Bits(v_r_size) : 0;

    if (mvscale)
        PMV[1] >>= 1; /* DIV 2 */

    decode_motion_vector(&PMV[1], v_r_size, motion_code, motion_residual,
                         full_pel_vector);

    if (mvscale)
        PMV[1] <<= 1;

    if (dmv)
        dmvector[1] = Get_dmvector();
}

/* calculate motion vector component */
/* ISO/IEC 13818-2 section 7.6.3.1: Decoding the motion vectors */
/* Note: the arithmetic here is more elegant than that which is shown
   in 7.6.3.1.  The end results (PMV[][][]) should, however, be the same.  */

static void
decode_motion_vector(int *pred, int r_size, int motion_code, int motion_residual,
                     int full_pel_vector /* MPEG-1 (ISO/IEC 11172-1) support */)
{
    int lim, vec;

    r_size = r_size % 32;
    lim = 16 << r_size;
    vec = full_pel_vector ? (*pred >> 1) : (*pred);

    if (motion_code > 0)
    {
        vec += ((motion_code - 1) << r_size) + motion_residual + 1;
        if (vec >= lim)
            vec -= lim + lim;
    }
    else if (motion_code < 0)
    {
        vec -= ((-motion_code - 1) << r_size) + motion_residual + 1;
        if (vec < -lim)
            vec += lim + lim;
    }
    *pred = full_pel_vector ? (vec << 1) : vec;
}

int main()
{
	int i, j, k;
	int main_result;
	int PMV[2][2][2];
	int dmvector[2];
	int motion_vertical_field_select[2][2];
	int s, motion_vector_count, mv_format, h_r_size, v_r_size, dmv, mvscale;

	main_result = 0;
	evalue = 0;
	System_Stream_Flag = 0;
	s = 0;
	motion_vector_count = 1;
	mv_format = 0;
	h_r_size = 200;
	v_r_size = 200;
	dmv = 0;
	mvscale = 1;
	for (i = 0; i < 2; i++)
	{
		dmvector[i] = 0;
		for (j = 0; j < 2; j++)
		{
			motion_vertical_field_select[i][j] = inmvfs[i][j];
			for (k = 0; k < 2; k++)
				PMV[i][j][k] = inPMV[i][j][k];
		}
	}

	Initialize_Buffer();
	#pragma kernel
	motion_vectors(PMV, dmvector, motion_vertical_field_select, s,
				   motion_vector_count, mv_format, h_r_size, v_r_size, dmv,
				   mvscale);

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
		{
			main_result +=
				(motion_vertical_field_select[i][j] != outmvfs[i][j]);
			for (k = 0; k < 2; k++)
				main_result += (PMV[i][j][k] != outPMV[i][j][k]);
		}

	//printf("%d\n", main_result);
	return main_result;
}
