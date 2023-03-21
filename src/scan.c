#include "scan.h"
#include <libft.h>
#include <limits.h>

t_fdf_token_kind fdf_kind(char c)
{
    if (c == ' ' || c == ',')
        return (FDF_SPACE);
    else if (c == '\n')
        return (FDF_NEWL);
    else if (c == '+' || c == '-' || ft_isalnum(c))
        return (FDF_INT);
    else
        return (FDF_EOF);
}

t_fdf_token *fdf_scan(const char *src, t_fdf_token *tok)
{
    if (!tok->expr)
	tok->expr = src;
    else
	tok->expr = tok->expr + tok->len;
    tok->len = 0;
    tok->kind = fdf_kind(tok->expr[0]);
    while (tok->kind != FDF_EOF && tok->kind == fdf_kind(tok->expr[tok->len]))
    {
        tok->len++;
    }
    return (tok);
}

size_t fdf_atoi_prefix(t_fdf_token tok, int *sign, const char **base)
{
    size_t i;

    i = 0;
    *sign = 1;
    while (i < tok.len && (tok.expr[i] == '+' || tok.expr[i] == '-'))
    {
        if (tok.expr[i] == '-')
            *sign = -*sign;
        i++;
    }
    if (i < tok.len - 2 && tok.expr[i] == '0' && tok.expr[i + 1] == 'x')
    {
        *base = "0123456789abcdef";
        i += 2;
    }
    else
        *base = "0123456789";
    return (i);
}

int fdf_atoi(t_fdf_token tok)
{
    size_t i;
    int sign;
    int pow;
    int x;
    const char *base;
    
    if (tok.kind != FDF_INT)
        return (INT_MIN);
    i = fdf_atoi_prefix(tok, &sign, &base);
    x = 0;
    pow = 1;
    while (i < tok.len)
    {
        x += ft_strchr(base, ft_tolower(tok.expr[i])) - base;
        x *= pow;
        pow *= 10;
        i++;
    }
    return x;
}
