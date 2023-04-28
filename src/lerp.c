#include "util.h"

int fdf_lerp(int t0, int t1, int x0, int x1)
{
    return (x0 + ((float)t0 / t1) * (x1 - x0));
}

int fdf_lerp_rgb(int t0, int t1, int x0, int x1)
{
    const float t = (float)t0 / t1;
    const int r = ((x0 >> 16) & 255) + t * (((x1 >> 16) & 255) - ((x0 >> 16) & 255));
    const int g = ((x0 >> 8) & 255) + t * (((x1 >> 8) & 255) - ((x0 >> 8) & 255));
    const int b = (x0 & 255) + t * ((x1 & 255) - (x0 & 255));
    return (r << 16 | g << 8 | b);
}

/* int fdf_lerp_rgb(int t0, int t1, int x0, int x1) */
/* { */
/*     const t_rgb rgb0 = {.r = (x0 >> 16) & 255, .g = (x0 >> 8) & 255, .b = x0 & 255}; */
/*     const t_rgb rgb1 = {.r = (x1 >> 16) & 255, .g = (x1 >> 8) & 255, .b = x1 & 255}; */
/*     int lerp; */

/*     lerp = 0; */
/*     lerp |= fdf_lerp(t0, t1, rgb0.r, rgb1.r); */
/*     lerp |= fdf_lerp(t0, t1, rgb0.g, rgb1.g) << 8; */
/*     lerp |= fdf_lerp(t0, t1, rgb0.b, rgb1.b) << 16; */
/*     return (lerp); */
/* } */
