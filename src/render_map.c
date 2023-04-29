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

void fdf_draw_segment_fast(t_fdf_renderer *rdr, t_fdf_camera *cam, t_fdf_vertex *a, t_fdf_vertex *b)
{
    t_vec2f d;
    t_vec2f p;
    int step;
    int s;

    if (!a || !b || !a->ok || !b->ok || !cohen_sutherland(&a->point, &b->point, *cam->w, *cam->h))
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
        fdf_draw_pixel_rgb(rdr, (t_vec2){p.x, p.y}, fdf_lerp_rgb(s, step, a->color, b->color));
        p.x += d.x;
        p.y += d.y;
    }
}

void fdf_draw_segment_antialiased(t_fdf_renderer *rdr, t_fdf_camera *cam, t_fdf_vertex *a, t_fdf_vertex *b)
{
    t_vec2f d;
    t_vec2f p;
    t_vec2f f;
    int step;
    int s;

    if (!a || !b || !a->ok || !b->ok || !cohen_sutherland(&a->point, &b->point, *cam->w, *cam->h))
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
        int color = fdf_lerp_rgb(s, step, a->color, b->color);
        fdf_draw_pixel_rgba(rdr, (t_vec2){p.x, p.y}, color, 1.0);
        if (d.x > 0)
            fdf_draw_pixel_rgba(rdr, (t_vec2){p.x + 1, p.y}, color, 0.7 * (floorf(p.x + 1) - p.x));
        else if (d.x < 0)
            fdf_draw_pixel_rgba(rdr, (t_vec2){p.x - 1, p.y}, color, 0.7 * (p.x - ceilf(p.x - 1)));
        if (d.y < 0)
            fdf_draw_pixel_rgba(rdr, (t_vec2){p.x, p.y - 1}, color, 0.7 * (p.y - ceilf(p.y - 1)));
        else if (d.y > 0)
            fdf_draw_pixel_rgba(rdr, (t_vec2){p.x, p.y + 1}, color, 0.7 * (floorf(p.y + 1) - p.y));
        p.x += d.x;
        p.y += d.y;
    }
}

t_fdf_vertex fdf_vertex_make(t_fdf_map *map, t_fdf_camera *cam, t_mat4 *mvp, t_vec2 pos)
{
    t_vec4f v;
    t_vec2 p;
    int height;
    int color;
    float dist;

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
    if (v.z < -v.w)
        return (t_fdf_vertex){.ok = 0};
    p.x = (1 - v.x / v.w) * 0.5 * (*cam->w);
    p.y = (1 - v.y / v.w) * 0.5 * (*cam->h);
    dist = powf(v.x + cam->pos.x, 2) + powf(v.y + cam->pos.y, 2) + powf(v.z + cam->pos.z, 2);
    return (t_fdf_vertex){1, color, p, (t_vec2){pos.x, pos.y}, dist};
}

/* void fdf_draw_map(t_fdf_renderer *render, t_fdf_map *map, t_fdf_camera *cam) */
/* { */
/*     int x; */
/*     int y; */
/*     t_fdf_vertex v[3]; */
/*     t_mat4 *mvp; */

/*     y = 0; */
/*     mvp = fdf_camera_mvp(camera); */
/*     while (y < map->h) */
/*     { */
/*         x = 0; */
/*         while (x < map->w) */
/*         { */
/*             v[0] = fdf_vertex_make(map, camera, mvp, (t_vec2){x, y}); */
/*             if (v[0].ok) */
/*             { */
/*                 v[1] = fdf_vertex_make(map, camera, mvp, (t_vec2){x + 1, y}); */
/*                 v[2] = fdf_vertex_make(map, camera, mvp, (t_vec2){x, y + 1}); */
/*                 fdf_draw_segment_antialiased(render, camera, &v[0], &v[1]); */
/*                 fdf_draw_segment_antialiased(render, camera, &v[0], &v[2]); */
/*             } */
/*             x++; */
/*         } */
/*         y++; */
/*     } */
/* } */

/* t_fdf_vertex *fdf_sort_vertices(t_fdf_renderer *render, t_fdf_map *map) */
/* { */
/*     t_fdf_vertex tmp; */
/*     size_t i; */
/*     size_t size; */

/*     i = 0; */
/*     size = map->w * map->h; */
/*     while (i < size - 1) */
/*     { */
/*         if (render->vs[i].dist > render->vs[i + 1].dist) */
/*         { */
/*             tmp = render->vs[i]; */
/*             render->vs[i] = render->vs[i + 1]; */
/*             render->vs[i + 1] = tmp; */
/*             i = 0; */
/*         } */
/*         else */
/*         { */
/* 	    i++; */
/*         } */
/*     } */
/*     return (render->vs); */
/* } */

#include <stdlib.h>

int vertex_dist_cmp(const void *v_, const void *w_)
{
    const t_fdf_vertex *v = *(t_fdf_vertex **)v_;
    const t_fdf_vertex *w = *(t_fdf_vertex **)w_;
    
    return (w->dist - v->dist);
}

t_fdf_vertex **fdf_sort_vertices(t_fdf_renderer *render, t_fdf_map *map)
{
    t_fdf_vertex *tmp;
    size_t i;
    size_t size;

    size = map->w * map->h;
    i = 0;
    while (i < size)
    {
	render->vss[i] = &render->vs[i];
	i++;
    }
    qsort(render->vss, size, sizeof(t_fdf_vertex *), vertex_dist_cmp);
    return (render->vss);
}


void fdf_draw_map(t_fdf_renderer *render, t_fdf_map *map, t_fdf_camera *cam)
{
    int x;
    int y;
    size_t i;
    t_mat4 *mvp;

    y = 0;
    mvp = fdf_camera_mvp(cam);
    while (y < map->h)
    {
        x = 0;
        while (x < map->w)
        {
            render->vs[y * map->w + x] = fdf_vertex_make(map, cam, mvp, (t_vec2){x, y});
            if (x < map->w - 1)
                render->vs[y * map->w + x].adjacent[0] = &render->vs[y * map->w + (x + 1)];
            if (y < map->h - 1)
                render->vs[y * map->w + x].adjacent[1] = &render->vs[(y + 1) * map->w + x];
            x++;
        }
        y++;
    }
    render->vss = fdf_sort_vertices(render, map);
    i = 0;
    while (i < map->w * map->h)
    {
        fdf_draw_segment_antialiased(render, cam, render->vss[i], render->vss[i]->adjacent[0]);
        fdf_draw_segment_antialiased(render, cam, render->vss[i], render->vss[i]->adjacent[1]);
        i++;
    }
}
