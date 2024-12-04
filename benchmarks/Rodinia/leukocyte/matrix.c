#include "matrix.h"
#define NULL ((void *)0)

// Reimplementations of some functions from the original library
double m_get_val(MAT *m, int i, int j)
{
    return m->me[i][j];
}

MAT *m_get(int m, int n)
{
    MAT *result = (MAT *)malloc(sizeof(MAT));
    result->m = m;
    result->n = n;
    result->me = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++)
        result->me[i] = (double *)malloc(n * sizeof(double));
    return result;
}

m_set_val(MAT *m, int i, int j, double val)
{
    m->me[i][j] = val;
}

double v_get_val(VEC *v, int i)
{
    return v->ve[i];
}

VEC *v_get(int size)
{
    VEC *result = (VEC *)malloc(sizeof(VEC));
    result->dim = size;
    result->ve = (double *)calloc(size, sizeof(double));
    return result;
}

v_set_val(VEC *v, int i, double val)
{
    v->ve[i] = val;
}

VEC *get_row(const MAT *mat, unsigned int row, VEC *vec)
{
    unsigned int i;

    if (mat == (MAT *)NULL)
        error(8, "get_row");
    if (row >= mat->m)
        error(10, "get_row");
    // if (vec == (VEC *)NULL || vec->dim < mat->n)
    //     vec = v_resize(vec, mat->n);

    for (i = 0; i < mat->n; i++)
        vec->ve[i] = mat->me[row][i];

    return (vec);
}

void m_free(MAT *m)
{
    for (int i = 0; i < m->m; i++)
        free(m->me[i]);
    free(m->me);
    free(m);
}

void v_free(VEC *v)
{
    free(v->ve);
    free(v);
}

MAT *mmtr_mlt(MAT *a, MAT *b, MAT *c)
{
    int i, j, k;
    double sum;

    if (a->n != b->m)
        error(1, "mmtr_mlt");

    if (c == (MAT *)NULL || c->m != a->m || c->n != b->n)
        error(2, "mmtr_mlt");

    for (i = 0; i < a->m; i++)
        for (j = 0; j < b->n; j++)
        {
            sum = 0.0;
            for (k = 0; k < a->n; k++)
                sum += a->me[i][k] * b->me[k][j];
            c->me[i][j] = sum;
        }
    return c;
}

MAT *mtrm_mlt(MAT *a, MAT *b, MAT *c)
{
    int i, j, k;
    double sum;

    if (a->n != b->m)
        error(1, "mtrm_mlt");

    if (c == (MAT *)NULL || c->m != a->m || c->n != b->n)
        error(2, "mtrm_mlt");

    for (i = 0; i < a->m; i++)
        for (j = 0; j < b->n; j++)
        {
            sum = 0.0;
            for (k = 0; k < a->n; k++)
                sum += a->me[i][k] * b->me[j][k];
            c->me[i][j] = sum;
        }
    return c;
}

MAT *m_copy(MAT *a, MAT *b)
{
    int i, j;

    if (a->m != b->m || a->n != b->n)
        error(4, "m_copy");

    for (i = 0; i < a->m; i++)
        for (j = 0; j < a->n; j++)
            b->me[i][j] = a->me[i][j];

    return b;
}

MAT *m_inverse(MAT *a, MAT *b)
{
    int i, j;
    double d;

    if (a->m != a->n)
        error(3, "m_inverse");

    if (b == (MAT *)NULL || b->m != a->m || b->n != a->n)
        error(4, "m_inverse");

    b = m_copy(a, b);

    for (i = 0; i < a->m; i++)
        for (j = 0; j < a->n; j++)
            b->me[i][j] = 0.0;

    return b;
}

MAT *m_zero(MAT *a)
{
    int i, j;

    for (i = 0; i < a->m; i++)
        for (j = 0; j < a->n; j++)
            a->me[i][j] = 0.0;

    return a;
}

// end of mock functions