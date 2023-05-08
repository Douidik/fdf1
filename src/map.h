#ifndef FDF_MAP_H
# define FDF_MAP_H

# include <stddef.h>

typedef struct s_fdf_map
{
    int *buf;
    int w;
    int h;
    int min;
    int max;
    int amplitude;
} t_fdf_map;

t_fdf_map *fdf_map_free(t_fdf_map *map);
void fdf_map_dump(t_fdf_map *map);

#endif
