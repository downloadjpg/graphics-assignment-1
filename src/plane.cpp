#include "plane.h"
#include <iostream>

Plane::Plane(vec3 position, vec3 _normal) {
    origin = position;
    normal = _normal;
    
}

Surface::HitRecord Plane::intersection(Ray& ray) {
    float denominator = dot(normal, ray.direction);
    if (abs(denominator) > 0.0001) {
        float distance = dot(normal, origin - ray.origin) / denominator;
        if (distance > 0) {
            return HitRecord{
                .hit = true,
                .distance = distance,
                .normal = normal,
                .position = ray.origin + distance * ray.direction,
                .surface = this
            };
        }

    }

    return HitRecord::Miss();
}
/*
Surface::IntersectionData Plane::funkyIntersection(Ray& ray) {
    float denominator = dot(normal, ray.direction);
    if (abs(denominator) > 0.0001) {
        float distance = dot(normal, origin - ray.origin) / denominator;
        if (distance > 0) {
            vec3 reflectedDirection = ray.direction - 2 * dot(ray.direction, normal) * normal;
            return IntersectionData{
                .hit = true,
                .normal = reflectedDirection,
                .distance = distance
            };
        }
    }
    return Surface::IntersectionData{
        .hit = false,
        .normal = vec3(0,0,0,0),
        .distance = 0.0f
    };
}*/