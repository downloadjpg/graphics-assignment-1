#include "sphere.h"
#include "plane.h"
#include "camera.h"
//#include "light.h"
#include <glm/vec4.hpp>
#include <vector>

struct Scene {
    float time = 0.0f;
    Camera* camera;
    std::vector<Surface*> surfaces;
    //std::vector<Light*> lights = {};
    Color rayColor(Ray& ray);
public:
    Scene();
    ~Scene();
    unsigned char* renderImage(const int width, const int height);
};