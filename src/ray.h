#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
using namespace glm;

struct Ray {
    vec3 origin;
    vec3 direction; // should ALWAYS be normalized!

    Ray(vec3 _origin, vec3 _direction) {
        origin = _origin;
        direction = normalize(_direction);
    }

    Ray() = default;
};