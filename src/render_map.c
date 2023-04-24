#include "camera.h"
#include "map.h"
#include "render.h"
#include "window.h"
#include "util.h"
#include <math.h>
#include <mlx.h>
#include <libft.h>

typedef struct s_fdf_vertex
{
    int ok;
    int height;
    t_vec2 point;
    t_vec2 pos;
} t_fdf_vertex;

void fdf_draw_segment(t_fdf_renderer *rdr, t_fdf_vertex *a, t_fdf_vertex *b)
{
    t_vec2f d;
    t_vec2f p;
    int step;
    int s;

    if (!a->ok || !b->ok)
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
    while (s < step)
    {
        fdf_draw_pixel(rdr, (t_vec2){p.x, p.y}, 0xcafe);
        p.x += d.x;
        p.y += d.y;
        s++;
    }
}

t_fdf_vertex fdf_vertex_make(t_fdf_map *map, t_fdf_camera *camera, t_mat4 *mvp, t_vec2 pos)
{
    t_vec4f v;
    t_vec2 p;
    int height;

    p = (t_vec2){0};
    if (pos.x >= map->w || pos.y >= map->h)
        return (t_fdf_vertex){.ok = 0};
    height = -map->buf[pos.y * map->w + pos.x];
    v.x = pos.x;
    v.y = height;
    v.z = pos.y;
    v.w = 1.0f;
    v = mat4_mul_vec4(*mvp, v);
    if (v.w <= 0)
        return (t_fdf_vertex){.ok = 0};
    p.x = ((v.x / v.w + 1) / 2) * (*camera->w);
    p.y = ((v.y / v.w + 1) / 2) * (*camera->h);
	p.x = fdf_clamp(p.x, 0, *camera->w);
	p.y = fdf_clamp(p.y, 0, *camera->h);
    return (t_fdf_vertex){1, height, p, (t_vec2){pos.x, pos.y}};
}

void fdf_draw_map(t_fdf_renderer *render, t_fdf_map *map, t_fdf_camera *camera)
{
    int x;
    int y;
    t_fdf_vertex v[3];
    t_mat4 *mvp;

    y = 0;
    mvp = fdf_camera_mvp(camera);
    ft_memset(render->stream, 0, *camera->w * *camera->h * (render->bpp / 8));
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
                fdf_draw_segment(render, &v[0], &v[1]);
                fdf_draw_segment(render, &v[0], &v[2]);
            }
            x++;
        }
        y++;
    }
}
