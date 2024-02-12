#include <glm/vec3.hpp>
#include "colorf.h"
struct Light {
    vec3 origin;    // for point light
    vec3 direction; // for directional light (ambient uses neither of these)
    ColoredLight lightColor; // includes intensity information
    enum class Type {
        AMBIENT,
        POINT,
        DIRECTIONAL
    } type = Type::POINT;
};