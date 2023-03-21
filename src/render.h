#ifndef FDF_RENDER_H
#define FDF_RENDER_H

#include "vec.h"

struct s_fdf_map;
struct s_fdf_window;
struct s_fdf_camera;

typedef enum e_fdf_endian
{
    FDF_ENDIAN_SMALL = 0,
    FDF_ENDIAN_BIG = 1
} t_fdf_endian;

typedef struct s_fdf_renderer
{
    void *mlx;
    void *image;
    struct s_fdf_window *window;
    char *stream;
    int bpp;
    int w;
    t_fdf_endian endian;
    t_vec2 pos_ui;
} t_fdf_renderer;

t_fdf_renderer *fdf_renderer_new(void *mlx, struct s_fdf_window *window);
t_fdf_renderer *fdf_renderer_free(t_fdf_renderer *render);
void fdf_draw_pixel(t_fdf_renderer *render, t_vec2 pos, int rgb);
void fdf_draw_map(t_fdf_renderer *render, struct s_fdf_map *map, struct s_fdf_camera *camera);
void fdf_draw_ui(t_fdf_renderer *render, int color, char *str);
void fdf_render_image(t_fdf_renderer *render);

#endif
