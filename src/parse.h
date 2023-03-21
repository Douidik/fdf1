#ifndef FDF_PARSE_H
#define FDF_PARSE_H

struct s_fdf_map;

typedef struct s_fdf_parser
{
    char *src;
    int fd;
} t_fdf_parser;

t_fdf_parser *fdf_parser_new(int fd);
t_fdf_parser *fdf_parser_free(t_fdf_parser *parser);
struct s_fdf_map *fdf_parse_map(t_fdf_parser *parser, struct s_fdf_map *map);

#endif
