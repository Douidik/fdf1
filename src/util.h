#ifndef FDF_UTIL_H
#define FDF_UTIL_H

#include "vec.h"
#include <stddef.h>

#define PI 3.14159f

char *fdf_read_fd(int fd);
void fdf_min_max(int *array, size_t size, int *min, int *max);
const char *fdf_filename(const char *fp);
int fdf_abs(int x);
int fdf_lerp(int t0, int t1, int x0, int x1);
int fdf_lerp_rgb(int t0, int t1, int x0, int x1);
void *fdf_memset(void *dst, int value, size_t size);
void *fdf_qsort(void *xs, size_t len, size_t size, int (*cmp)(const void *, const void *));

#endif
