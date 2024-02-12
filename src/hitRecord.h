#pragma once
#include <glm/vec3.hpp>

using namespace glm;

class Surface; // Forward declaration

struct HitRecord {
    bool hit;
    float distance;
    vec3 normal;
    vec3 position; //  redundant, as it's the distance times the ray's direction, but often needed eventually.
    Surface* surface; // oh boy now we're getting into memory management

    static HitRecord Miss() { return HitRecord{
        .hit = false,
        .distance = std::numeric_limits<float>::max(),
        .position = vec3(0,0,0),
        .surface = nullptr};
    }
};