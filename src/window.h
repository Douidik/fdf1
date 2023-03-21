#ifndef FDF_WINDOW_H
#define FDF_WINDOW_H

typedef struct s_fdf_window
{
    void *mlx;
    void *impl;    
    char *name;
    int w;
    int h;
} t_fdf_window;

t_fdf_window *fdf_window_new(void *mlx, int w, int h, const char *fp);
t_fdf_window *fdf_window_free(t_fdf_window *window);

#endif
