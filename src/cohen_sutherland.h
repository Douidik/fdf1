#ifndef FDF_COHEN_SUTHERLAND_H
#define FDF_COHEN_SUTHERLAND_H

#include "vec.h"

int cohen_sutherland(t_vec2 *a, t_vec2 *b, int w, int h);
t_vec2 segment_intersect_x(t_vec2 *a, t_vec2 *b, int y);
t_vec2 segment_intersect_y(t_vec2 *a, t_vec2 *b, int x);

#endif
