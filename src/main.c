#include "fdf.h"
#include "map.h"
#include "parse.h"
#include <fcntl.h>
#include <ft_printf.h>
#include <unistd.h>

#include "util.h"

/* int int_cmp(const void *a, const void *b) */
/* { */
/*     return (*(int *)a) < (*(int *)b); */
/* } */

/* int main_(int argc, char **argv) */
/* { */
/*     int ints[8] = {8, 73, 654423, 1, 44, 343, 21, 1}; */
/*     for (size_t i = 0; i < 8; i++) */
/*         ft_printf("%d,", ints[i]); */

/*     fdf_qsort(ints, 8, sizeof(int), int_cmp); */
/*     ft_printf("\n"); */

/*     for (size_t i = 0; i < 8; i++) */
/*         ft_printf("%d,", ints[i]); */
/*     return (0); */
/* } */

/* #include <stdlib.h> */
/* #include <time.h> */

/* void array_print(int *array, size_t size) */
/* { */
/*     ft_printf("["); */
/*     for (size_t i = 0; i < size; i++) */
/*     { */
/*         ft_printf("%d", array[i]); */
/*         if (i < size - 1) */
/*             ft_printf(", "); */
/*     } */
/*     ft_printf("]\n"); */
/* } */

/* int array_sorted(int *array, size_t size) */
/* { */
/*     for (size_t i = 0; i < size - 1; i++) */
/*     { */
/*         if (array[i] > array[i + 1]) */
/*             return 0; */
/*     } */
/*     return 1; */
/* } */

/* void qsort_benchmark(const char *name, int *array, size_t len, */
/*                      void (*func)(void *, size_t, size_t, int (*cmp)(const void *, const void *))) */
/* { */
/*     struct timespec start, end; */
/*     clock_gettime(CLOCK_MONOTONIC, &start); */
/*     { */
/*         func(array, len, sizeof(int), int_cmp); */
/*     } */
/*     clock_gettime(CLOCK_MONOTONIC, &end); */
/*     unsigned long start_ms = start.tv_sec * 1E3 + start.tv_nsec / 1E6; */
/*     unsigned long end_ms = end.tv_sec * 1E3 + end.tv_nsec / 1E6; */
/*     unsigned long elapsed_ms = end_ms - start_ms; */
/*     ft_printf("%s: %d ms\n", name, elapsed_ms); */
/* } */

/* int main(int argc, char **argv) */
/* { */
/*     if (argc < 2) */
/*         return 1; */
/*     size_t samples = ft_atoi(argv[1]); */
/*     srand(time(NULL)); */
/*     int *std_array = ft_calloc(samples, sizeof(int)); */
/*     int *fdf_array = ft_calloc(samples, sizeof(int)); */
/*     for (size_t i = 0; i < samples; i++) */
/*     { */
/*         int random = rand(); */
/*         std_array[i] = rand(); */
/*         fdf_array[i] = rand(); */
/*     } */

/*     qsort_benchmark("fdf", std_array, samples, &fdf_qsort); */
/*     qsort_benchmark("std", std_array, samples, &qsort); */
/* } */

int main(int argc, char **argv)
{
    t_fdf_parser *parser;
    t_fdf_map map;
    t_fdf *fdf;
    int fd;

    ft_printf("fdf - wireframe rasterizer\n");
    if (argc < 2)
        return (ft_printf("No input file specified\n"), 1);
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return (ft_printf("Cannot open file from '%s'\n", argv[1]), 1);
    parser = fdf_parser_new(fd);
    close(fd);
    if (!parser)
        return (ft_printf("Cannot read map from file '%s'\n", argv[1]), 1);
    if (!fdf_parse_map(parser, &map))
        return (fdf_parser_free(parser), ft_printf("Cannot parse map from file '%s'", argv[1]), 1);
    /* fdf_map_dump(&map); */
    fdf = fdf_new(&map, argv[1]);
    fdf_parser_free(parser);
    if (!fdf)
        return (fdf_map_free(&map), 1);
    fdf_run(fdf);
    fdf_free(fdf);
    fdf_map_free(&map);
    return (0);
}
