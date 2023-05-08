#include "cohen_sutherland.h"

typedef enum e_viewport_region
{
    LEFT = 1 << 0,
    RIGHT = 1 << 1,
    BOTTOM = 1 << 2,
    TOP = 1 << 3,
} t_viewport_region;

int viewport_region(t_vec2 *v, int w, int h)
{
    int region;

    region = 0;
    if (v->x < 0)
        region |= LEFT;
    else if (v->x >= w)
        region |= RIGHT;
    if (v->y < 0)
        region |= BOTTOM;
    else if (v->y >= h)
        region |= TOP;
    return (region);
}

int cohen_sutherland(t_vec2 *a, t_vec2 *b, int w, int h)
{
    int region_a;
    int region_b;
    int region;
    t_vec2 *v;

    region_a = viewport_region(a, w, h);
    region_b = viewport_region(b, w, h);
    if ((region_a | region_b) == 0)
        return (1);
    if ((region_a & region_b) != 0)
        return (0);
    if (region_a > region_b)
        region = region_a, v = a;
    else
        region = region_b, v = b;
    if (region & LEFT)
        *v = segment_intersect_y(a, b, 0);
    else if (region & RIGHT)
        *v = segment_intersect_y(a, b, w - 1);
    else if (region & BOTTOM)
        *v = segment_intersect_x(a, b, 0);
    else if (region & TOP)
        *v = segment_intersect_x(a, b, h - 1);
    return (cohen_sutherland(a, b, w, h));
}

/* int cohen_sutherland(t_vec2 *a, t_vec2 *b, int w, int h) */
/* { */
/*     int region_a; */
/*     int region_b; */

/*     region_a = viewport_region(a, w, h); */
/*     region_b = viewport_region(b, w, h); */
/*     if ((region_a | region_b) == 0) */
/*         return (1); */
/*     if ((region_a & region_b) != 0) */
/*         return (0); */
/*     if (region_a > region_b) */
/*     { */
/*         if (region_a & LEFT) */
/*             *a = segment_intersect_y(a, b, 0); */
/*         else if (region_a & RIGHT) */
/*             *a = segment_intersect_y(a, b, w); */
/*         if (region_a & BOTTOM) */
/*             *a = segment_intersect_x(a, b, 0); */
/*         else if (region_a & TOP) */
/*             *a = segment_intersect_x(a, b, h); */
/*     } */
/*     else */
/*     { */
/*         if (region_b & LEFT) */
/*             *b = segment_intersect_y(a, b, 0); */
/*         else if (region_b & RIGHT) */
/*             *b = segment_intersect_y(a, b, w); */
/*         else if (region_b & BOTTOM) */
/*             *b = segment_intersect_x(a, b, 0); */
/*         else if (region_b & TOP) */
/*             *b = segment_intersect_x(a, b, h); */
/*     } */

/*     return (cohen_sutherland(a, b, w, h)); */
/* } */
