#include "util.h"
#include <stdlib.h>

void fdf_qsort_swap(t_fdf_qsort_ctx *ctx, void *a, void *b)
{
    uint8_t swap_byte;
    uint16_t swap_word;
    uint32_t swap_dword;
    uint64_t swap_qword;
    size_t i;

    i = 0;
    while (i + sizeof(uint64_t) <= ctx->size)
    {
        swap_qword = (*(uint64_t *)(a + i));
        (*(uint64_t *)(a + i)) = (*(uint64_t *)(b + i));
        (*(uint64_t *)(b + i)) = swap_qword;
        i += sizeof(uint64_t);
    }
    while (i + sizeof(uint32_t) <= ctx->size)
    {
	swap_dword = (*(uint32_t *)(a + i));
        (*(uint32_t *)(a + i)) = (*(uint32_t *)(b + i));
        (*(uint32_t *)(b + i)) = swap_dword;
        i += sizeof(uint32_t);
    }
    while (i + sizeof(uint16_t) <= ctx->size)
    {
	swap_word = (*(uint16_t *)(a + i));
        (*(uint16_t *)(a + i)) = (*(uint16_t *)(b + i));
        (*(uint16_t *)(b + i)) = swap_word;
        i += sizeof(uint16_t);
    }
    while (i < ctx->size)
    {
        swap_byte = (*(uint8_t *)(a + i));
        (*(uint8_t *)(a + i)) = (*(uint8_t *)(b + i));
        (*(uint8_t *)(b + i)) = swap_byte;
        i++;
    }
}

ssize_t fdf_qsort_partition(t_fdf_qsort_ctx *ctx, ssize_t begin, ssize_t end)
{
    ssize_t i;
    ssize_t j;
    void *a;
    void *b;
    void *pivot;

    i = begin - 1;
    j = begin;
    pivot = &ctx->array[end * ctx->size];
    while (j < end)
    {
	b = &ctx->array[j * ctx->size];
	if (ctx->cmp(b, pivot))
	{
	    i++;
	    a = &ctx->array[i * ctx->size];
	    fdf_qsort_swap(ctx, a, b);
	}
	j++;
    }
    a = &ctx->array[(i + 1) * ctx->size];
    fdf_qsort_swap(ctx, a, pivot);
    return (i + 1);
}

void fdf_qsort_impl(t_fdf_qsort_ctx *ctx, ssize_t begin, ssize_t end)
{
    if (begin < end)
    {
        ssize_t pivot_index = fdf_qsort_partition(ctx, begin, end);
        fdf_qsort_impl(ctx, begin, pivot_index - 1);
        fdf_qsort_impl(ctx, pivot_index + 1, end);
    }
}

void fdf_qsort(void *array, size_t len, size_t size, int (*cmp)(const void *, const void *))
{
    t_fdf_qsort_ctx ctx = {array, len, size, cmp};
    fdf_qsort_impl(&ctx, 0, (ssize_t)len - 1);
}
