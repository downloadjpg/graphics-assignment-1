#include "scene.h"

class Renderer {
public:
    Camera* camera;
    Scene* scene;
    // Writes the output buffer at the specified address, representing the final image.
    unsigned char* renderImage(const int width, const int height); // in pixels!

    Renderer(Camera* camera, Scene* scene);
    ~Renderer();
    

private:
    // Returns the color of a ray, given the scene.
    vec3 colorRay(Ray& ray, int depth);
    // Returns the closest intersection with a surface.
    HitRecord closestIntersectionWithSurface(Ray& ray, float tMin, float tMax);

    // Graphics shenanigans
    unsigned char* outputBuffer;
    void writeOutputBuffer(unsigned char* image, int width, int height);
    // float* radianceBuffer;
    // void writeRadianceBuffer()
    // void applyToneMap()
    // void writeImageFile()
};
