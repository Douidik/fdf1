#include "util.h"
#include <stdlib.h>

void fdf_swap(void *xs, size_t i, size_t j)
{
    void *tmp;

    /* tmp = xs + i ; */
    /* xs[i] = xs[j]; */
    /* xs[j] = tmp; */
}

void *fdf_qsort(void *xs, size_t len, size_t size, int (*cmp)(const void *, const void *))
{
    qsort(xs, len, size, cmp);
    return (xs);
}
