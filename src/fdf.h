#ifndef FDF_FDF_H
#define FDF_FDF_H

#include "camera.h"

struct s_fdf_map;
struct s_fdf_window;

typedef struct s_fdf
{
    void *mlx;
	const char *filename;
    struct s_fdf_map *map;
    struct s_fdf_window *window;
    struct s_fdf_renderer *render;
    t_fdf_camera camera;
    int show_help;
} t_fdf;

t_fdf *fdf_new(struct s_fdf_map *map, const char *fp);
t_fdf *fdf_free(t_fdf *fdf);
void fdf_run(t_fdf *fdf);

#endif 
