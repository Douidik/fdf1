#ifndef FDF_SCAN_H
#define FDF_SCAN_H

#include <stddef.h>

typedef enum e_fdf_token_kind
{
    FDF_SPACE = 1 << 0,
    FDF_NEWL = 1 << 1,
    FDF_INT = 1 << 2,
    FDF_EOF = 1 << 3
} t_fdf_token_kind;

typedef struct s_fdf_token
{
    const char *expr;
    size_t len;
    t_fdf_token_kind kind;
} t_fdf_token;

t_fdf_token *fdf_scan(const char *src, t_fdf_token *tok);
int fdf_atoi(t_fdf_token tok);

#endif
