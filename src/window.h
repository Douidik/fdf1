#ifndef FDF_WINDOW_H
#define FDF_WINDOW_H

#include "vec.h"

typedef struct s_fdf_window
{
    void *mlx;
    void *impl;    
    char *name;
    int w;
    int h;
} t_fdf_window;

t_fdf_window *fdf_window_new(void *mlx, int w, int h, const char *filename);
t_fdf_window *fdf_window_free(t_fdf_window *window);
t_vec2 fdf_window_mouse_get(t_fdf_window *window);
t_vec2 fdf_window_mouse_set(t_fdf_window *window, t_vec2 pos);

#endif
