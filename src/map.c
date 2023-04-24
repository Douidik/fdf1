#include "map.h"
#include <ft_printf.h>
#include <stdlib.h>

t_fdf_map *fdf_map_free(t_fdf_map *map)
{
    if (map != NULL && map->buf != NULL)
	free(map->buf);
    return (NULL);
}

int fdf_map_prec(t_fdf_map *map)
{
    int prec;
    int xs[4];

    prec = 1;
    xs[0] = map->min;
    xs[1] = map->max;
    xs[2] = map->w;
    xs[3] = map->h;
    while (xs[0] || xs[1] || xs[2] || xs[3])
    {
        prec++;
        xs[0] /= 10;
        xs[1] /= 10;
        xs[2] /= 10;
        xs[3] /= 10;
    }
    return (prec);
}

void fdf_map_dump(t_fdf_map *map)
{
    int x;
    int y;
    int prec;
xo
    x = 0;
    y = 0;
    prec = fdf_map_prec(map);
    ft_printf("%*s", prec, ":");
    while (x < map->w)
        ft_printf("%*d", prec, x++);
    while (y < map->h)
    {
        ft_printf("\n%*d", prec, y);
        x = 0;
        while (x < map->w)
        {
            ft_printf("%*d", prec, map->buf[y * map->w + x]);
            x++;
        }
        y++;
    }
    ft_printf("\n");
}
