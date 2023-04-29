#include "util.h"

void fdf_swap(void **xs, size_t i, size_t j)
{
    void *tmp;

    tmp = xs[i];
    xs[i] = xs[j];
    xs[j] = tmp;
}

void fdf_sort(void **xs, size_t i, size_t j, int (*cmp)(void *, void *))
{
}
