#include "plane.h"
#include <iostream>

Plane::Plane(vec4 position, vec4 _normal) {
    origin = position;
    normal = _normal;
}

RenderObject::IntersectionData Plane::intersection(Ray& ray) {
    //std::cout << "check";
    float denominator = dot(normal, ray.direction);
    if (abs(denominator) > 0.0001) {
        float distance = dot(normal, origin - ray.origin) / denominator;
        if (distance > 0) {
             std::cout << "hit!";
            return IntersectionData{
                .hit = true,
                .normal = normal,
                .distance = distance
            };
        }

    }
    return IntersectionData{
        .hit = false,
        .normal = vec4(0,0,0,0),
        .distance = 0.0f
    };
}
