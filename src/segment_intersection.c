#include "cohen_sutherland.h"
#include <float.h>

// From: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
// slope = (y1 - y0) / (x1 - x0)
// slope = (b->y - a->y) / (b->x - a->x)
// y = y0 + slope * (xm - x0), where xm is xmin or xmax

float slope(t_vec2 *a, t_vec2 *b)
{
    return ((float)(b->y - a->y) / (b->x - a->x));
}

t_vec2 segment_intersect_x(t_vec2 *a, t_vec2 *b, int y)
{
    return ((t_vec2){a->x + (1 / slope(a, b)) * (y - a->y), y});
}

t_vec2 segment_intersect_y(t_vec2 *a, t_vec2 *b, int x)
{
    return ((t_vec2){x, a->y + slope(a, b) * (x - a->x)});
}
