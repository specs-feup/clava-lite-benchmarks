#pragma once

#define NULL ((void *)0)
#define Real float

/* vector definition */
typedef struct
{
    unsigned int dim, max_dim;
    Real *ve;
} VEC;

/* matrix definition */
typedef struct
{
    unsigned int m, n;
    unsigned int max_m, max_n, max_size;
    Real **me, *base; /* base is base of alloc'd mem */
} MAT;

// Reimplementations of some functions from the original library
double m_get_val(MAT *m, int i, int j);

MAT *m_get(int m, int n);

m_set_val(MAT *m, int i, int j, double val);

double v_get_val(VEC *v, int i);

VEC *v_get(int size);

v_set_val(VEC *v, int i, double val);

VEC *get_row(const MAT *mat, unsigned int row, VEC *vec);

void m_free(MAT *m);

void v_free(VEC *v);

MAT *mmtr_mlt(MAT *a, MAT *b, MAT *c);

MAT *mtrm_mlt(MAT *a, MAT *b, MAT *c);

MAT *m_inverse(MAT *a, MAT *b);

MAT *m_zero(MAT *a);

// end of mock functions