#pragma once
#include <vector>
#include <limits>
#include <glm/vec3.hpp>

#include "renderer.h"

using namespace glm;

class Camera;
class Surface;
struct Light;
struct Ray;
struct HitRecord;

class Scene {
    const float EPSILON = std::numeric_limits<float>::epsilon();
public:
    // A scene is composed of a camera, a list of surfaces and a list of lights.
    Camera* camera;
    std::vector<Surface*> surfaces;
    std::vector<Light*> lights;
    Renderer* renderer;

    // Constructor and destructor.
    Scene();
    ~Scene();

    unsigned char* renderImage(const int width, const int height); // in pixel
private:
    // Returns the color of a ray, given the scene.
    vec3 colorRay(Ray& ray, int depth);
    // Maps a radiance buffer so each value is between 0 and 1. (Reinhard tone mapping).
    void applyToneMap(vec3* radianceBuffer, int width, int height, float whitePoint);
};
