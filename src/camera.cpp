#include "camera.h"

Camera::Camera(vec3 _origin, vec3 _up, vec3 _forward) {
    origin = _origin;
    v = _up;
    w = _forward * -1.0f;
    u = cross(v, w); // points to the right, (or left, if you're looking the same way as the camera!)
};

void Camera::move(vec3 direction, float delta) {
    float moveSpeed = 5.0f;
    origin += direction * moveSpeed * delta;
}


// Redefines u, v, and w to look at a new position. 
void Camera::lookAt(vec3 position) {
    // Of course it's really looking in the opposite direction, as -w is forward.
    w = normalize(-(position - origin)); // forward
    u = normalize(cross(vec3(0,1,0), w)); // right
    v = cross(w, u); // up
    // NOTE: we assume a global up direction direction to maintain orthogonality
}


// Generate a ray for a given pixel, depends on projection type, origin, and basis vectors.
Ray Camera::generateRay(int i, int j) {
    if (projectionType == orthographic) {
        return generateOrthographicRay(i, j);
    } else {
        return generatePerspectiveRay(i, j);
    }
}


Ray Camera::generateOrthographicRay(int i, int j) {
    float u_scale = viewPlane.l + (viewPlane.r - viewPlane.l) * (i + 0.5) / (float)imageWidth;
    float v_scale = viewPlane.l + (viewPlane.r - viewPlane.l) * (j + 0.5) / (float)imageHeight;
    
    Ray ray;
    ray.direction = -w;
    ray.origin = origin + u_scale * u + v_scale * v;
    
    return ray;
}


Ray Camera::generatePerspectiveRay(int i, int j) {
    float u_scale = viewPlane.l + (viewPlane.r - viewPlane.l) * (i + 0.5) / (float)imageWidth;
    float v_scale = viewPlane.b + (viewPlane.t - viewPlane.b) * (j + 0.5) / (float)imageHeight;
    float& d = perspectiveDistance;

    Ray ray;
    ray.direction = normalize(-d * w + u_scale * u + v_scale * v);
    ray.origin = origin;
    
    return ray;
}


void Camera::setProjectionType(ProjectionType type) {
    projectionType = type;
}



