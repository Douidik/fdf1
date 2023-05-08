#ifndef FDF_VEC_H
#define FDF_VEC_H

typedef struct s_vec2
{
    int x;
    int y;
} t_vec2;

typedef struct s_vec2f
{
    float x;
    float y;
} t_vec2f;

typedef struct s_vec3 {
    int x;
    int y;
    int z;
} t_vec3;

typedef struct s_vec3f
{
    float x;
    float y;
    float z;
} t_vec3f;

typedef struct s_vec4f
{
    float x;
    float y;
    float z;
    float w;
} t_vec4f;

float vec2_len(t_vec2 v);
// TODO! use vec3f on map rendering
float vec3f_dist(t_vec3f a, t_vec3f b);
float vec3f_len(t_vec3f v);
t_vec3f vec3f_norm(t_vec3f v);
t_vec3f vec3f_scale(t_vec3f v, float s);

#endif
