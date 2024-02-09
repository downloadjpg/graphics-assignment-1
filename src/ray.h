#include <glm/vec3.hpp>
#pragma once

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3 _origin = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 _direction = glm::vec3(0.0f, 0.0f, 0.0f)) {
        origin = _origin;
        direction = _direction;
    }
};