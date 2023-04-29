#ifndef FDF_FDF_H
#define FDF_FDF_H

#include "camera.h"

struct s_fdf_map;
struct s_fdf_window;

typedef enum s_fdf_ui_mode
{
    FDF_HELP = 0,
    FDF_DEBUG = 1,
    FDF_CINEMATIC = 2,
    FDF_UI_MODE_COUNT
} t_fdf_ui_mode;

typedef struct s_fdf
{
    void *mlx;
    const char *filename;
    struct s_fdf_map *map;
    struct s_fdf_window *window;
    struct s_fdf_renderer *render;
    t_fdf_camera cam;
    t_fdf_ui_mode ui_mode;
} t_fdf;

t_fdf *fdf_new(struct s_fdf_map *map, const char *fp);
t_fdf *fdf_free(t_fdf *fdf);
void fdf_run(t_fdf *fdf);

#endif
