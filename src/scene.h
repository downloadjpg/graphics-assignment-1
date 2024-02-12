#pragma once
#include <vector>
#include <limits>
#include <glm/vec3.hpp>

#include "ray.h"

using namespace glm;

class Camera;
class Surface;
struct Light;
struct HitRecord;

class Scene {
    const float EPSILON = std::numeric_limits<float>::epsilon();
public:
    // A scene is composed of a camera, a list of surfaces and a list of lights.
    Camera* camera;
    std::vector<Surface*> surfaces;
    std::vector<Light*> lights;

    // Constructor and destructor.
    Scene();
    ~Scene();

    unsigned char* renderImage(const int width, const int height); // in pixel

    // Minimum and maximum values for ray intersection.
    const float tMin = 0.001f;
    const float tMax = 10000.0f;
private:
    // The time of the scene.
    float time = 0.0f;


    // Returns the color of a ray, given the scene.
    vec3 colorRay(Ray& ray, int depth);

    // Returns the closest intersection with a surface.
    HitRecord closestIntersectionWithSurface(Ray& ray, std::vector<Surface*> surfaces);

    // Writes a radiance buffer at the specified address, representing the raw amount/color of light at each pixel.
    // Returns the maximum white point in the image, which is used for tone mapping.
    float writeRadianceBuffer(vec3* buffer, int width, int height);

    // Maps a radiance buffer so each value is between 0 and 1. (Reinhard tone mapping).
    void applyToneMap(vec3* radianceBuffer, int width, int height, float whitePoint);
};