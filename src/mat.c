#include "mat.h"
#include <math.h>

t_mat4 mat4_identity()
{
    t_mat4 mat;

    mat = (t_mat4){0};
    mat.m[0][0] = 1.0f;
    mat.m[1][1] = 1.0f;
    mat.m[2][2] = 1.0f;
    mat.m[3][3] = 1.0f;
    return (mat);
}

t_mat4 mat4_translation(t_mat4 mat, t_vec3f t)
{
    t_mat4 tsl;

    tsl = mat4_identity();
    tsl.m[0][3] = t.x;
    tsl.m[1][3] = t.y;
    tsl.m[2][3] = t.z;
    return (mat4_mul_mat4(mat, tsl));
}

t_mat4 mat4_rotation_x(float x)
{
    t_mat4 mat;
    float sin;
    float cos;

    mat = mat4_identity();
    sin = sinf(x);
    cos = cosf(x);
    mat.m[1][1] = +cos;
    mat.m[1][2] = -sin;
    mat.m[2][1] = +sin;
    mat.m[2][2] = +cos;
    return (mat);
}

t_mat4 mat4_rotation_y(float y)
{
    t_mat4 mat;
    float sin;
    float cos;

    mat = mat4_identity();
    sin = sinf(y);
    cos = cosf(y);
    mat.m[0][0] = +cos;
    mat.m[0][2] = +sin;
    mat.m[2][0] = -sin;
    mat.m[2][2] = +cos;
    return (mat);
}

/* t_mat4 mat4_rotation(t_mat4 mat, t_vec3f r) */
/* { */
/*     t_mat4 rot; */

/*     rot = mat4_identity(); */
/*     rot = mat4_mul_mat4(rot, mat4_rotation_x(r.x)); */
/*     rot = mat4_mul_mat4(rot, mat4_rotation_y(r.y)); */
/*     mat = mat4_mul_mat4(mat, rot); */
/*     return (mat); */
/* } */

t_mat4 mat4_rotation(t_mat4 mat, t_vec3f r)
{
    t_mat4 rot;

    mat = mat4_mul_mat4(mat, mat4_rotation_x(r.x));
    mat = mat4_mul_mat4(mat, mat4_rotation_y(r.y));
    return (mat);
    
    rot = mat4_identity();
    rot = mat4_mul_mat4(rot, mat);
    rot = mat4_mul_mat4(rot, mat4_rotation_x(r.x));
    rot = mat4_mul_mat4(rot, mat4_rotation_y(r.y));
    /* mat = mat4_mul_mat4(mat, rot); */
    return (rot);
}

t_mat4 mat4_scale(t_mat4 mat, t_vec3f s)
{
    mat.m[0][0] *= s.x;
    mat.m[1][1] *= s.y;
    mat.m[2][2] *= s.z;
    return (mat);
}
