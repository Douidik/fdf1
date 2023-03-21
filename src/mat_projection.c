#include "mat.h"
#include <math.h>

t_mat4 mat4_perspective(t_perspective_plane p)
{
    t_mat4 mat;

    mat = mat4_identity();
    mat.m[0][0] = p.ratio * (1 / tan(p.fov / 2));
    mat.m[1][1] = (1 / tan(p.fov / 2));
    mat.m[2][2] = p.zfar / (p.zfar - p.znear);
    mat.m[2][3] = (-p.zfar * p.znear) / (p.zfar - p.znear);
    mat.m[3][2] = 1.0f;
    return (mat);
}
