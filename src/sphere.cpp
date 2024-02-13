#include "surface.h"
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <algorithm>
// TODO: Review this!

Sphere::Sphere(vec3 _origin, float _radius){
    origin = _origin;
    radius = _radius;
};

HitRecord Sphere::intersection(Ray& ray) {
    // Some help taken from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
    vec3 oc = ray.origin - origin; // 'oc' being o-center
    float a = 1; //= dot(ray.direction, ray.direction); this should always equal 1, since D is normalized.
    float b = 2.0f * dot(oc, ray.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) 
        return HitRecord::Miss();

    // Normal will be the vector from the center of the sphere to the point of intersection
    discriminant = sqrt(discriminant);
    float distance = (-b / 2.0f) - discriminant;
    if (distance <= 0) {
        float distance2 = (-b / 2.0f) + discriminant; // if we're inside the sphere, we want the second intersection
        if (distance2 <= 0)
            return HitRecord::Miss();
        distance = distance2;
    }
            

    vec3 position = ray.origin + distance * ray.direction;
    vec3 normal = normalize(position - origin);
    
    return HitRecord{
        .hit = true,
        .distance =  distance, // this might be wrong!
        .normal = normal,
        .position = position,
        .surface = this
    };
};