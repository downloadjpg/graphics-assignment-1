#pragma once
#include <glm/vec3.hpp>
using namespace glm;

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

    Light* newAmbientLight(vec3 position, vec3 color, float intensity);
    Light* newDirectionalLight(vec3 direction, vec3 color, float intensity);
    Light* newPointLight(vec3 origin, vec3 color, float intensity);
    vec3 directionToLight(vec3 position);
    float distanceToLight(vec3 position);
    //float intensityAt(vec3 position); // if i ever decide to do attenuation or falloff or something.
};