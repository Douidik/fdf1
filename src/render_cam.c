#include "camera.h"
#include "render.h"
#include "window.h"
#include <math.h>
#include <stdio.h>

uint32_t fdf_shader_dotted(t_fdf_renderer *render, t_vec2 p, int s[2], void *data)
{
    if (s[0] % 4 <= 2)
    	return 0;
    else
	return 0xffffffff;
}

void fdf_draw_cam(t_fdf_renderer *render, t_fdf_camera *cam)
{
    t_vec2 points[2];
    
    if (cam->input.type == CAM_IDLE)
	    return;
    points[0].x = cam->input.anchor.x;
    points[0].y = cam->input.anchor.y;
    points[1].x = fdf_window_mouse_get(render->window).x;
    points[1].y = fdf_window_mouse_get(render->window).y;
    fdf_draw_segment(render, points, fdf_shader_dotted, cam);
}
