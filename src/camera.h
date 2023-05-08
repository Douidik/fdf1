#ifndef FDF_CAMERA_H
#define FDF_CAMERA_H

#include "mat.h"

#define FDF_CAM_SENSITIVITY 0.25

struct s_fdf_window;
struct s_fdf_map;
struct s_fdf;

typedef enum s_fdf_camera_input_type
{
    CAM_IDLE,
    CAM_MOVE,
    CAM_LOOK,
    CAM_ZOOM,
} t_fdf_camera_input_type;

typedef struct s_fdf_camera_input
{
    t_fdf_camera_input_type type;
    t_vec3f prev;
    t_vec2 anchor;
    int key;
} t_fdf_camera_input;

typedef struct s_fdf_camera
{
    t_fdf_camera_input input;
    float height;
    int obsolete;
    t_vec3f pos;
    t_vec3f rot;
    t_vec3f scale;
    t_mat4 m;
    t_mat4 v;
    t_mat4 p;
    t_mat4 mvp;
    t_perspective_plane persp;
} t_fdf_camera;

t_fdf_camera fdf_camera_new(float fov, struct s_fdf_window *window);
t_mat4 *fdf_camera_mvp(t_fdf_camera *cam);

void fdf_camera_update(t_fdf_camera *cam, struct s_fdf_window *window);
void fdf_camera_move(t_fdf_camera *cam, t_vec2 motion);
void fdf_camera_look(t_fdf_camera *cam, t_vec2 motion);
void fdf_camera_zoom(t_fdf_camera *cam, float amount);

int fdf_camera_keypress_hook(int k, struct s_fdf *fdf);
int fdf_camera_keyrelease_hook(int k, struct s_fdf *fdf);
int fdf_camera_buttonpress_hook(int b, int x, int y, struct s_fdf *fdf);

#endif
