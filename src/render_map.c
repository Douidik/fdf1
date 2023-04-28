#include "camera.h"
#include "cohen_sutherland.h"
#include "map.h"
#include "render.h"
#include "util.h"
#include "window.h"
#include <libft.h>
#include <math.h>
#include <mlx.h>

/* #define FDF_COLOR_MIN (0x000000) */
/* #define FDF_COLOR_MAX (0xffffff) */

#define FDF_COLOR_MIN (0x6666ee)
#define FDF_COLOR_MAX (0xee22ee)

/* #define FDF_COLOR_MIN (0x5C258D) */
/* #define FDF_COLOR_MAX (0x4389A2) */

typedef struct s_fdf_vertex
{
    int ok;
    int color;
    t_vec2 point;
    t_vec2 pos;
} t_fdf_vertex;

void fdf_draw_segment(t_fdf_renderer *rdr, t_fdf_camera *camera, t_fdf_vertex *a, t_fdf_vertex *b)
{
    t_vec2f d;
    t_vec2f p;
    int step;
    int s;

    if (!a->ok || !b->ok || !cohen_sutherland(&a->point, &b->point, *camera->w, *camera->h))
        return;
    d.x = (b->point.x - a->point.x);
    d.y = (b->point.y - a->point.y);
    step = fmaxf(fabsf(d.x), fabsf(d.y));
    if (step != 0)
    {
        d.x /= step;
        d.y /= step;
    }
    p = (t_vec2f){a->point.x, a->point.y};
    s = 0;
    while (s++ < step)
    {
        fdf_draw_pixel(rdr, (t_vec2){round(p.x), round(p.y)}, fdf_lerp_rgb(s, step, a->color, b->color));
        p.x += d.x;
        p.y += d.y;
    }
}

t_fdf_vertex fdf_vertex_make(t_fdf_map *map, t_fdf_camera *camera, t_mat4 *mvp, t_vec2 pos)
{
    t_vec4f v;
    t_vec2 p;
    int height;
    int color;

    p = (t_vec2){0};
    if (pos.x >= map->w || pos.y >= map->h)
        return (t_fdf_vertex){.ok = 0};
    height = map->buf[pos.y * map->w + pos.x];
    color = fdf_lerp_rgb(height + map->min, map->amplitude + map->min, FDF_COLOR_MIN, FDF_COLOR_MAX);
    v.x = pos.x;
    v.y = height;
    v.z = pos.y;
    v.w = 1.0;
    v = mat4_mul_vec4(*mvp, v);
    /* if (v.z < -v.w) */
        /* return (t_fdf_vertex){.ok = 0}; */
    p.x = (1 - v.x / v.w) * 0.5 * (*camera->w);
    p.y = (1 - v.y / v.w) * 0.5 * (*camera->h);
    return (t_fdf_vertex){1, color, p, (t_vec2){pos.x, pos.y}};
}

void fdf_draw_map(t_fdf_renderer *render, t_fdf_map *map, t_fdf_camera *camera)
{
    int x;
    int y;
    t_fdf_vertex v[3];
    t_mat4 *mvp;

    y = 0;
    mvp = fdf_camera_mvp(camera);
    while (y < map->h)
    {
        x = 0;
        while (x < map->w)
        {
            v[0] = fdf_vertex_make(map, camera, mvp, (t_vec2){x, y});
            if (v[0].ok)
            {
                v[1] = fdf_vertex_make(map, camera, mvp, (t_vec2){x + 1, y});
                v[2] = fdf_vertex_make(map, camera, mvp, (t_vec2){x, y + 1});
                fdf_draw_segment(render, camera, &v[0], &v[1]);
                fdf_draw_segment(render, camera, &v[0], &v[2]);
            }
            x++;
        }
        y++;
    }
}
