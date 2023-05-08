#ifndef FDF_UTIL_H
#define FDF_UTIL_H

#include "vec.h"
#include <stddef.h>
#include <stdint.h>
#include <libft.h>

#define PI 3.14159f

typedef struct s_fdf_qsort_ctx
{
    uint8_t *array;
    size_t len;
    size_t size;
    int (*cmp)(const void *, const void *);
} t_fdf_qsort_ctx;

char *fdf_read_fd(int fd);
void fdf_min_max(int *array, size_t size, int *min, int *max);
const char *fdf_filename(const char *fp);
int fdf_abs(int x);
int fdf_lerp(int t0, int t1, int x0, int x1);
uint32_t fdf_lerp_rgb(int t0, int t1, int32_t x0, int32_t x1);
void *fdf_memset(void *dst, int value, size_t size);
void *fdf_memcpy(void *dst, void *src, size_t size);
void fdf_qsort(void *array, size_t len, size_t size, int (*cmp)(const void *, const void *));

#endif
