#include "window.h"
#include "util.h"
#include <libft.h>
#include <mlx.h>
#include <stdlib.h>

t_fdf_window *fdf_window_new(void *mlx, int w, int h, const char *filename)
{
    t_fdf_window *window;

    window = ft_calloc(1, sizeof(t_fdf_window));
    if (!window)
		return (NULL);
    window->mlx = mlx;
    window->w = w;
    window->h = h;
    window->name = ft_strjoin("fdf - ", filename);
    if (!window->name)
		return (fdf_window_free(window));
    window->impl = mlx_new_window(mlx, w, h, window->name);
    return (window);
}

t_fdf_window *fdf_window_free(t_fdf_window *window)
{
    if (window != NULL)
    {
		if (window->name != NULL)
			free(window->name);
		if (window->impl != NULL)
			mlx_destroy_window(window->mlx, window->impl);
		free(window);
    }
    return (NULL);
}
