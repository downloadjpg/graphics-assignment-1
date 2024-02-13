#include "light.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

vec3 Light::directionToLight(vec3 position) {
    if (type == Type::POINT) {
        return glm::normalize(origin - position);
    }
    if (type == Type::DIRECTIONAL) {
        return -direction;
    }
    return vec3(0,0,0);
}

float Light::distanceToLight(vec3 position) {
    if (type == Type::POINT) {
        return glm::length(origin - position);
    }
    return std::numeric_limits<float>::max();
}

Light* Light::newAmbientLight(vec3 position, vec3 color, float intensity) {
    Light* light = new Light();
    light->type = Type::AMBIENT;
    light->color = vec3(1,1,1);
    light->intensity = 0.2f;
    return light;
}

Light* Light::newDirectionalLight(vec3 direction, vec3 color, float intensity) {
    Light* light = new Light();
    light->type = Type::DIRECTIONAL;
    light->direction = normalize(direction);
    light->color = color;
    light->intensity = intensity;
    return light;
}

Light* Light::newPointLight(vec3 origin, vec3 color, float intensity) {
    Light* light = new Light();
    light->type = Type::POINT;
    light->origin = origin;
    light->color = color;
    light->intensity = intensity;
    return light;
}