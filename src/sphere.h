#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "ray.h"
#include "surface.h"

using namespace glm;

class Sphere : public Surface{
public:
    vec3 origin;
    float radius;

    HitRecord intersection(Ray& ray);
    Sphere(vec3 _origin, float _radius);

};