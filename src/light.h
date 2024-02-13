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

    static Light* newAmbientLight(vec3 _color, float _intensity) {
        return new Light(vec3(0, 0, 0), vec3(0, 0, 0), _color, _intensity, Type::AMBIENT);
    }

    static Light* newPointLight(vec3 _origin, vec3 _color, float _intensity) {
        return new Light(_origin, vec3(0, 0, 0), _color, _intensity, Type::POINT);
    }

    static Light* newDirectionalLight(vec3 _direction, vec3 _color, float _intensity) {
        return new Light(vec3(0, 0, 0), _direction, _color, _intensity, Type::DIRECTIONAL);
    }

    Light(vec3 _origin, vec3 _direction, vec3 _color, float _intensity, Type _type = Type::POINT) {
        origin = _origin;
        direction = normalize(_direction);
        color = _color;
        intensity = _intensity;
        type = _type;
    }
};