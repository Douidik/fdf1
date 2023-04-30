#ifndef FDF_CAMERA_H
#define FDF_CAMERA_H

#include "mat.h"

struct s_fdf_window;
struct s_fdf_map;

typedef struct s_fdf_camera
{
    int obsolete;
    t_vec3f origin;
    t_vec3f pos;
    t_vec3f rot;
    float zoom;
    t_mat4 m;
    t_mat4 v;
    t_mat4 p;
    t_mat4 mvp;
    t_perspective_plane persp;
    int *w;
    int *h;
} t_fdf_camera;

t_fdf_camera fdf_camera_new(float fov, struct s_fdf_window *window, struct s_fdf_map *map);
t_mat4 *fdf_camera_mvp(t_fdf_camera *cam);
void fdf_camera_translate(t_fdf_camera *cam, t_vec3f motion);
void fdf_camera_rotate(t_fdf_camera *cam, t_vec3f motion);
void fdf_camera_zoom(t_fdf_camera *cam, float amount);

#endif
