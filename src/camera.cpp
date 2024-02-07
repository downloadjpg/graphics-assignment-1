#include "camera.h"

Camera::Camera(vec4 _origin, vec4 _up, vec4 _forward) {
    origin = _origin;
    v = _up;
    w = _forward * -1.0f;
    u = vec4(cross(vec3(v), vec3(w)), 0);
};

Ray Camera::generateRay(int i, int j) {
    float u_scale = l + (r-l) * (i+0.5) / (float)imageWidth;
	float v_scale = l + (r-l) * (j+0.5) / (float)imageHeight;
	
	Ray ray;
    ray.direction = -w;
	ray.origin = origin + u_scale * u + v_scale * v;
	
	return ray;
}