#ifndef FDF_MAT_H
#define FDF_MAT_H

#include "vec.h"

typedef struct s_mat4
{
    float m[4][4];
} t_mat4;

typedef struct s_perspective_plane
{
    float fov;
    float ratio;
    float znear;
    float zfar;
} t_perspective_plane;

typedef struct s_orthographic_plane
{
    float left;
    float right;
    float bottom;
} t_orthographic_plane;

t_mat4 mat4_identity();

t_mat4 mat4_perspective(t_perspective_plane p);
t_mat4 mat4_orthographic(t_perspective_plane plane);

t_mat4 mat4_translation(t_mat4 mat, t_vec3f t);
t_mat4 mat4_rotation(t_mat4 mat, t_vec3f r);
t_mat4 mat4_scale(t_mat4 mat, t_vec3f s);

t_mat4 mat4_mul_mat4(t_mat4 a, t_mat4 b);
t_mat4 mat4_mul_scalar(t_mat4 mat, float s);
t_vec4f mat4_mul_vec4(t_mat4 mat, t_vec4f vec);

#endif
