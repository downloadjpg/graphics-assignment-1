#pragma once
#include <glm/vec3.hpp>
using namespace glm;

class Camera;
class Scene;
class Ray;
class HitRecord;

class Renderer {
public:
    Camera* camera;
    Scene* scene;
    int maxDepth = 2;

    unsigned char* renderImage();

    Renderer(Scene* scene);
    ~Renderer();
    

private:
    // Returns the color of a ray, given the scene.
    vec3 colorRay(Ray& ray, int depth);
    // Returns the closest intersection with a surface.
    HitRecord closestIntersectionWithSurface(Ray &ray, 
        float tMin = std::numeric_limits<float>::epsilon(), 
        float tMax = std::numeric_limits<float>::max());

    // Graphics shenanigans

    const int width = 512; // in pixels!
    const int height = 512;
    // Have a single location for the output buffer
    unsigned char* outputBuffer;
    vec3* radianceBuffer;
    void writeOutputBuffer();
    void writeRadianceBuffer();

    void radianceBufferToImage();

};
