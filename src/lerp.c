#include "util.h"

int fdf_lerp(int t0, int t1, int x0, int x1)
{
    return (x0 + ((float)t0 / t1) * (x1 - x0));
}

/* #include <math.h> */

/* uint32_t fdf_lerp_rgb(int t0, int t1, int32_t x0, int32_t x1) */
/* { */
/*     const float t = (float)t0 / t1; */
/*     uint8_t r[2]; */
/*     uint8_t g[2]; */
/*     uint8_t b[2]; */
/*     uint32_t rgb; */

/*     r[0] = x0 >> 16 & 255; */
/*     r[1] = x1 >> 16 & 255; */
/*     g[0] = x0 >> 8 & 255; */
/*     g[1] = x1 >> 8 & 255; */
/*     b[0] = x0 & 255; */
/*     b[1] = x1 & 255; */
/*     rgb = 0; */
/*     rgb |= ((uint32_t)(r[0] + t * (r[1] - r[0])) & 255) << 16; */
/*     rgb |= ((uint32_t)(g[0] + t * (g[1] - g[0])) & 255) << 8; */
/*     rgb |= ((uint32_t)(b[0] + t * (b[1] - b[0]))) & 255; */
/*     rgb |= 255 << 24; */
/*     return (rgb); */
/* } */

typedef union u_fdf_rgb {
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
    uint32_t v;
} t_fdf_rgb;

uint32_t fdf_lerp_rgb(int t0, int t1, int32_t x0, int32_t x1)
{
    const float t = (float)t0 / t1;
    const t_fdf_rgb a = (t_fdf_rgb){.v = x0};
    const t_fdf_rgb b = (t_fdf_rgb){.v = x1};
    t_fdf_rgb lerp;

    lerp.r = a.r + t * (b.r - a.r);
    lerp.g = a.g + t * (b.g - a.g);
    lerp.b = a.b + t * (b.b - a.b);
    lerp.a = 255;
    return (lerp.v);
}
