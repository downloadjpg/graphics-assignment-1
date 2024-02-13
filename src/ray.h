#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction; // should ALWAYS be normalized!

    Ray() = default;
    Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(glm::normalize(direction)) {}
};