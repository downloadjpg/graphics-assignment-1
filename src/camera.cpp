#include "camera.h"

Camera::Camera(vec4 _origin = vec4(0,0,0,1), vec4 _up = vec4(0,1,0,0), vec4 _forward = vec4(0,0,-1,0)) {
    origin = _origin;
    v = _up;
    w = _forward * -1.0f;
    u = vec4(cross(vec3(v), vec3(w)), 0);
};

Ray Camera::generateRay(int i, int j) {
    if (projectionType == orthographic) {
        return generateOrthographicRay(i, j);
    } else {
        return generatePerspectiveRay(i, j);
    }
}

Ray Camera::generateOrthographicRay(int i, int j) {
    float u_scale = l + (r-l) * (i+0.5) / (float)imageWidth;
	float v_scale = l + (r-l) * (j+0.5) / (float)imageHeight;
	
	Ray ray;
    ray.direction = -w;
	ray.origin = origin + u_scale * u + v_scale * v;
	
	return ray;
}

Ray Camera::generatePerspectiveRay(int i, int j) {
    float u_scale = l + (r-l) * (i+0.5) / (float)imageWidth;
	float v_scale = b + (t-b) * (j+0.5) / (float)imageHeight;
	float& d = perspectiveDistance;

	Ray ray;
    ray.direction = normalize(-d * w + u_scale * u + v_scale * v);
	ray.origin = origin;
	
	return ray;
}