#include "surface.h"
#include <limits>
#include <glm/geometric.hpp>

Plane::Plane(vec3 position, vec3 _normal) {
    origin = position;
    normal = _normal;
    material.albedo = vec3(0.1f,0.1f,0.2f);
}

HitRecord Plane::intersection(Ray& ray) {
    float denominator = dot(normal, ray.direction);
    //float insideNormal = sign(denominator);
    //denominator = abs(denominator);
    if (abs(denominator) > std::numeric_limits<float>::epsilon()) {
        float distance = dot(normal, origin - ray.origin) / denominator;
        if (distance > 0) {
            return HitRecord{
                .hit = true,
                .distance = distance,
                .normal = normal /** insideNormal*/,
                .position = ray.origin + distance * ray.direction,
                .surface = this
            };
        }

    }
    return HitRecord::Miss();
}