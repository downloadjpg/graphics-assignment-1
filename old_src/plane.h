#include "surface.h"
#include <glm/geometric.hpp>

using namespace glm;

class Plane : public Surface{
public:
    vec3 normal;
    Plane(vec3 position, vec3 _normal);
    HitRecord intersection(Ray& ray);
};