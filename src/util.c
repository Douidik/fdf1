#include "util.h"
#include <fcntl.h>
#include <libft.h>
#include <stdlib.h>

#define FDF_RDBUF_INIT 32
#define FDF_RDBUF_FACTOR 2

char *fdf_read_push(char *str, ssize_t len, ssize_t cap)
{
    char *new;

    new = malloc(cap + 1);
    if (new != NULL)
	ft_memcpy(new, str, len);
    free(str);
    return (new);
}

char *fdf_read_fd(int fd)
{
    ssize_t rdsize;
    size_t len;
    size_t cap;
    char *str;

    len = 0;
    cap = FDF_RDBUF_INIT;
    str = malloc(cap + 1);
    while (str != NULL && fd != -1)
    {
        rdsize = read(fd, str + len, cap - len);
	if (rdsize < 0)
	    return (free(str), NULL);
	if (rdsize != cap - len)
	{
	    str[len + rdsize] = '\0';
	    return (str);
	}
	len = len + rdsize;
	cap = cap * FDF_RDBUF_FACTOR;
	str = fdf_read_push(str, len, cap);
    }
    return (NULL);
}

void fdf_min_max(int *array, size_t size, int *min, int *max)
{
    size_t i;
    size_t j;

    if (!array || !size)
        return;
    i = 0;
    j = 0;
	if (max != NULL)
	{
		*max = array[0];
		while (i < size)
		{
			if (*max < array[i])
				*max = array[i];
			i++;
		}
	}
	if (min != NULL)
	{
		*min = array[0];
		while (j < size)
		{
			if (*min > array[j])
				*min = array[j];
			j++;
		}
	}
}

const char *fdf_filename(const char *fp)
{
    const char *name;

    name = fp;
    while (*fp != '\0')
    {
        if (*fp == '/' || *fp == '\\')
            name = fp + 1;
        fp++;
    }
    return (name);
}

int fdf_clamp(int x, int min, int max)
{
	if (x > max)
		return (max);
	if (x < min)
		return (min);
	return (x);
}
