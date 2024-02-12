#include <glm/vec3.hpp>
struct Light {
    vec3 origin;    // for point light
    vec3 direction; // for directional light (ambient uses neither of these)
    vec3 color;
    float intensity;
    enum class Type {
        AMBIENT,
        POINT,
        DIRECTIONAL
    } type = Type::POINT;
};