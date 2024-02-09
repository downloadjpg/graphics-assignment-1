#include "sphere.h"
#include "plane.h"
#include "camera.h"
//#include "light.h"
#include <glm/vec3.hpp>
#include <vector>

struct Light {
    vec3 origin;
    float intensity;

    Light(vec3 _origin, float _intensity) {
        origin = _origin;
        intensity = _intensity;
    }
};

struct Scene {
    float time = 0.0f;
    Camera* camera;
    std::vector<Surface*> surfaces;
    std::vector<Light*> lights = {};
    Color rayColor(Ray& ray);
    float accumulateLight(Ray& ray, Surface::HitRecord& intersection);
    Surface::HitRecord closestIntersectionWithSurface(Ray& ray);

public:
    Scene();
    ~Scene();
    unsigned char* renderImage(const int width, const int height);
};
