#include "vec.h"
#include <math.h>

int vec3f_null(t_vec3f v)
{
    return v.x == 0 && v.y == 0 && v.z == 0;
}

float vec3f_len(t_vec3f v)
{
    float slen;

    slen = v.x * v.x + v.y * v.y + v.z * v.z;
    if (slen != 0.0f)
        return (sqrtf(slen));
    return (slen);
}

t_vec3f vec3f_norm(t_vec3f v)
{
    float len;

    len = vec3f_len(v);
    if (len != 0.0f)
    {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return (v);
}

t_vec3f vec3f_scale(t_vec3f v, float s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return (v);
}
