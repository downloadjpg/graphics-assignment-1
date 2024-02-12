#include "camera.h"


Camera::Camera(vec3 _origin = vec3(0,0,0), vec3 _up = vec3(0,1,0), vec3 _forward = vec3(0,0,-1)) {
    origin = _origin;
    v = _up;
    w = _forward * -1.0f; // why the fuck have i multiplied by -1 three times,
    //                       i know -z is forward, but... i feel like i'm doing something wrong, 
    //                       are basis vectors just 'backward'?
    //
    //                       NOTE TO FUTURE SELF: please do not code while udner the influence
    //                           (...the influence of sleep deprivation)            
    //                                                          
    u = cross(v, w);
};


void Camera::move(vec3 direction, float delta) {
    float moveSpeed = 5.0f;
    origin += direction * moveSpeed * delta;
}

void Camera::lookAt(vec3 position){

     // flip here too! 'u' should point out the back of your head
    w = normalize(-(position - origin)); // forward
    u= normalize(cross(vec3(0,1,0), w)); //
    v = cross(u, v);

    // NOTE: we assume up is (0,1,0), assumes an 'up' direction to maintain orthogonality
}

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

void Camera::setProjectionType(ProjectionType type) {
    projectionType = type;
}