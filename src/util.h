#ifndef FDF_UTIL_H
#define FDF_UTIL_H

#include <stddef.h>

#define PI 3.14159f

char *fdf_read_fd(int fd);
void fdf_min_max(int *array, size_t size, int *min, int *max);
const char *fdf_filename(const char *fp);
float fdf_loopf(float x, float min, float max);

#endif