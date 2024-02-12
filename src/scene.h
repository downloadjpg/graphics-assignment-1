#include "sphere.h"
#include "plane.h"
#include "camera.h"
//#include "light.h"
#include <glm/vec3.hpp>
#include <vector>

struct Light {
    vec3 origin;    // for point light
    vec3 direction; // for directional light (ambient uses neither of these)
    float intensity;
    enum class Type {
        AMBIENT,
        POINT,
        DIRECTIONAL
    } type = Type::POINT;

    static Light* NewDirectionalLight(vec3 _direction, float intensity) {
        Light* light = new Light();
        light->direction = _direction;
        light->intensity = intensity;
        light->type = Type::DIRECTIONAL;
        return light;
    }

    static Light* NewPointLight(vec3 _origin, float intensity) {
        Light* light = new Light();
        light->origin = _origin;
        light->intensity = intensity;
        light->type = Type::POINT;
        return light;
    }

    static Light* NewAmbientLight(float intensity) {
        Light* light = new Light();
        light->intensity = intensity;
        light->type = Type::AMBIENT;
        return light;
    }
};

struct Scene {
    float time = 0.0f;
    Camera* camera;
    std::vector<Surface*> surfaces;
    std::vector<Light*> lights = {};
    ColorF rayColor(Ray& ray);
    float accumulateLight(Ray& ray, Surface::HitRecord& intersection);
    Surface::HitRecord closestIntersectionWithSurface(Ray& ray);

public:
    Scene();
    ~Scene();
    unsigned char* renderImage(const int width, const int height);
};
