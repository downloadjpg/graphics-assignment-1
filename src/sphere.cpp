#include "sphere.h"
#include <iostream>

Sphere::Sphere(vec4 _origin, float _radius){
    origin = _origin;
    radius = _radius;
    material.albedo = Color(200, 100, 150);
};

Surface::IntersectionData Sphere::intersection(Ray& ray) {
    // Some help taken from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
    vec4 oc = ray.origin - origin; // 'oc' being o-center
    float a = 1; //= dot(ray.direction, ray.direction); this should always equal 1, since D is normalized.
    float b = 2.0f * dot(oc, ray.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;


    // TODO: restructure intersectiondata to have no data when there's no hit! (wish i had rust enums :( )
    if (discriminant < 0) {
        return IntersectionData{ .hit = false};
    }

    // Normal will be the vector from the center of the sphere to the point of intersection
    float distance = (-b / 2.0f) - sqrt(discriminant);
    vec4 normal = normalize((ray.origin + distance * ray.direction) - origin);
    return IntersectionData{
        .hit = true,
        .normal = normal,
        .distance =  distance // this might be wrong!
    };
};