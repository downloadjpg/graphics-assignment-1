#include <glm/vec4.hpp>
#pragma once

struct Ray {
    glm::vec4 origin = glm::vec4(0, 0, 0, 1);
    glm::vec4 direction = glm::vec4(1, 0, 0, 0);
};