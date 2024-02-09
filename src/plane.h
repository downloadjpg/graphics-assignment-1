#include "surface.h"
#include <glm/geometric.hpp>

using namespace glm;

class Plane : public Surface{
public:
    vec4 normal;
    Plane(vec4 position, vec4 _normal);
    IntersectionData intersection(Ray& ray);
};