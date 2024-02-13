#include "surface.h"
#include <limits>

Triangle::Triangle(vec3 _v1, vec3 _v2, vec3 _v3) {
    v1 = _v1;
    v2 = _v2;
    v3 = _v3;

    origin = (v1 + v2 + v3) / 3.0f;
    normal = normalize(cross(v2 - v1, v3 - v1));
}

// How do we make sure the normal is the right way??? Gahh!!
HitRecord Triangle::intersection(Ray& ray) {
    // Möller–Trumbore intersection algorithm
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    // First, check if the ray is parallel to the triangle. This is identical to the plane intersection.

    vec3 edge1 = v2 - v1;
    vec3 edge2 = v3 - v1;
    vec3 rayCrossEdge2 = cross(ray.direction, edge2);
    float determinant = dot(edge1, rayCrossEdge2);
    if (abs(determinant) < std::numeric_limits<float>::epsilon()) {
    return HitRecord::Miss(); // This ray is parallel!
}

    float invDeterminant = 1.0f / determinant;

    // These are barycentric coordinates, I think.
    // Don't really understand it, but it'll check if the in the bounds of the triangle.
    vec3 s = ray.origin - v1; // s is the vector from the origin of the triangle to the origin of the ray
    float u = dot(s, rayCrossEdge2) * invDeterminant;
    if (u < 0 || u > 1) {
        return HitRecord::Miss();
    }

    vec3 sCrossEdge1 = cross(s, edge1);
    float v = dot(ray.direction, sCrossEdge1) * invDeterminant;
    if (v < 0 || u + v > 1) {
        return HitRecord::Miss();
    }

    // Now that we know the ray intersects the plane, we can calculate the distance
    float distance = invDeterminant * dot(edge2, sCrossEdge1);
    if (distance < std::numeric_limits<float>::epsilon()) {
        return HitRecord::Miss();
    }
    return HitRecord{
        .hit = true,
        .distance = distance,
        .normal = normal,
        .position = ray.origin + distance * ray.direction,
        .surface = this
    };
}