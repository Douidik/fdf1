#ifndef FDF_RENDER_H
#define FDF_RENDER_H

#include "vec.h"
#include <stddef.h>
#include <stdint.h>

struct s_fdf_map;
struct s_fdf_window;
struct s_fdf_camera;
struct s_fdf;

typedef enum e_fdf_render_technique
{
    FDF_SEGMENT_NAIVE,
    FDF_SEGMENT_BRESSENHAM,
    FDF_SEGMENT_ANTIALIASED,
} t_fdf_render_technique;

typedef enum e_fdf_endian
{
    FDF_ENDIAN_SMALL = 0,
    FDF_ENDIAN_BIG = 1
} t_fdf_endian;

typedef struct s_fdf_ui
{
    int x;
    int y;
    int h;
} t_fdf_ui;

typedef struct s_fdf_vertex
{
    int ok;
    uint32_t color;
    t_vec2 point;
    t_vec2 pos;
    float z;
    float height;
} t_fdf_vertex;

typedef struct s_fdf_renderer
{
    void *mlx;
    void *image;
    struct s_fdf_window *window;
    struct s_fdf_map *map;
    t_fdf_render_technique technique;
    char *stream;
    float *depth;
    int bpp;
    int w;
    t_fdf_endian endian;
    t_fdf_vertex *vs;
} t_fdf_renderer;

typedef uint32_t (*t_fdf_shader)(t_fdf_renderer *, t_vec2, int[2], void *);

t_fdf_renderer *fdf_renderer_new(void *mlx, struct s_fdf_window *window, struct s_fdf_map *map);
t_fdf_renderer *fdf_renderer_free(t_fdf_renderer *render);
void fdf_render_clear(t_fdf_renderer *render);
void fdf_draw_pixel(t_fdf_renderer *render, t_vec2 pos, uint32_t color);
void fdf_draw_pixel_rgba(t_fdf_renderer *render, t_vec2 pos, uint32_t rgb, float alpha);
void fdf_draw_map(t_fdf_renderer *render, struct s_fdf_camera *cam);
void fdf_draw_cam(t_fdf_renderer *render, struct s_fdf_camera *cam);
void fdf_draw_ui(t_fdf_renderer *render, t_fdf_ui *ui, uint32_t rgb, char *str);
void fdf_render_image(t_fdf_renderer *render);
void fdf_draw_segment(t_fdf_renderer *render, t_vec2 p[2], t_fdf_shader shader, void *data);

#endif
